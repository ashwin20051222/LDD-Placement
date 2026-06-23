#!/bin/bash

# GPIO IRQ Driver - Quick Start Script
# This script automates the build and setup process

set -e  # Exit on any error

COLOR_RED='\033[0;31m'
COLOR_GREEN='\033[0;32m'
COLOR_YELLOW='\033[1;33m'
COLOR_BLUE='\033[0;34m'
COLOR_NC='\033[0m' # No Color

# Helper functions
print_title() {
    echo -e "${COLOR_BLUE}${COLOR_NC}"
    echo -e "${COLOR_BLUE}${COLOR_NC}  $1"
    echo -e "${COLOR_BLUE}${COLOR_NC}"
}

print_success() {
    echo -e "${COLOR_GREEN}[]${COLOR_NC} $1"
}

print_error() {
    echo -e "${COLOR_RED}[]${COLOR_NC} $1"
}

print_warning() {
    echo -e "${COLOR_YELLOW}[!]${COLOR_NC} $1"
}

print_info() {
    echo -e "${COLOR_BLUE}[i]${COLOR_NC} $1"
}

# Check prerequisites
check_prerequisites() {
    print_title "Checking Prerequisites"
    
    # Check for gcc
    if ! command -v gcc &> /dev/null; then
        print_error "gcc not found. Install with: sudo apt install build-essential"
        exit 1
    fi
    print_success "gcc found"
    
    # Check for make
    if ! command -v make &> /dev/null; then
        print_error "make not found. Install with: sudo apt install build-essential"
        exit 1
    fi
    print_success "make found"
    
    # Check for kernel headers
    if [ ! -d "/lib/modules/$(uname -r)/build" ]; then
        print_error "Kernel headers not found"
        print_info "Install with: sudo apt install linux-headers-$(uname -r)"
        exit 1
    fi
    print_success "Kernel headers found"
    
    print_success "All prerequisites met"
    echo ""
}

# Build kernel module
build_module() {
    print_title "Building Kernel Module"
    
    print_info "Cleaning previous builds..."
    make clean > /dev/null 2>&1 || true
    
    print_info "Building gpio_irq_driver.ko..."
    if ! make > /dev/null 2>&1; then
        print_error "Build failed!"
        make
        exit 1
    fi
    
    if [ ! -f "gpio_irq_driver.ko" ]; then
        print_error "Module not found after build!"
        exit 1
    fi
    
    print_success "Module built successfully"
    echo ""
}

# Load kernel module
load_module() {
    print_title "Loading Kernel Module"
    
    # Check if already loaded
    if lsmod | grep -q gpio_irq_driver; then
        print_warning "Module already loaded, unloading first..."
        sudo rmmod gpio_irq_driver 2>/dev/null || true
    fi
    
    print_info "Loading gpio_irq_driver.ko..."
    if ! sudo insmod gpio_irq_driver.ko; then
        print_error "Failed to load module!"
        exit 1
    fi
    
    # Give system a moment to register the device
    sleep 1
    
    # Verify it's loaded
    if ! lsmod | grep -q gpio_irq_driver; then
        print_error "Module loaded but verification failed!"
        exit 1
    fi
    
    print_success "Module loaded successfully"
    
    # Show major number
    MAJOR=$(grep gpioirqdev /proc/devices | awk '{print $1}')
    print_info "Device major number: $MAJOR"
    echo ""
}

# Create device node
create_device_node() {
    print_title "Creating Device Node"
    
    MAJOR=$(grep gpioirqdev /proc/devices | awk '{print $1}')
    
    if [ -z "$MAJOR" ]; then
        print_error "Could not find device major number!"
        exit 1
    fi
    
    # Remove old device node if exists
    if [ -e /dev/gpioirqdev ]; then
        print_info "Removing old device node..."
        sudo rm -f /dev/gpioirqdev
    fi
    
    print_info "Creating /dev/gpioirqdev with major number $MAJOR..."
    if ! sudo mknod /dev/gpioirqdev c $MAJOR 0; then
        print_error "Failed to create device node!"
        exit 1
    fi
    
    print_info "Setting permissions..."
    sudo chmod 666 /dev/gpioirqdev
    
    print_success "Device node created: /dev/gpioirqdev"
    ls -la /dev/gpioirqdev
    echo ""
}

# Compile user applications
compile_apps() {
    print_title "Compiling User Applications"
    
    if [ -f gpio_user_app.c ]; then
        print_info "Compiling gpio_user_app..."
        if ! gcc -o gpio_user_app gpio_user_app.c; then
            print_error "Failed to compile gpio_user_app!"
            exit 1
        fi
        print_success "gpio_user_app compiled"
    fi
    
    if [ -f gpio_test_trigger.c ]; then
        print_info "Compiling gpio_test_trigger..."
        if ! gcc -o gpio_test_trigger gpio_test_trigger.c; then
            print_error "Failed to compile gpio_test_trigger!"
            exit 1
        fi
        print_success "gpio_test_trigger compiled"
    fi
    
    echo ""
}

# Show kernel logs
show_logs() {
    print_title "Kernel Logs"
    dmesg | tail -10
    echo ""
}

# Interactive menu
show_menu() {
    echo -e "${COLOR_BLUE}${COLOR_NC}"
    echo -e "${COLOR_BLUE}${COLOR_NC}        GPIO IRQ Driver - Main Menu"
    echo -e "${COLOR_BLUE}${COLOR_NC}"
    echo ""
    echo "  1) Full Setup (build, load, device, compile)"
    echo "  2) Build Only"
    echo "  3) Load Module"
    echo "  4) Unload Module"
    echo "  5) Create Device Node"
    echo "  6) Run User Application"
    echo "  7) Run Test Trigger"
    echo "  8) Show Kernel Logs"
    echo "  9) Cleanup"
    echo "  0) Exit"
    echo ""
}

# Full setup
full_setup() {
    print_title "Full Setup"
    check_prerequisites
    build_module
    load_module
    create_device_node
    compile_apps
    show_logs
    print_success "Setup Complete!"
}

# Unload module
unload_module() {
    print_title "Unloading Module"
    
    if lsmod | grep -q gpio_irq_driver; then
        print_info "Unloading gpio_irq_driver..."
        sudo rmmod gpio_irq_driver
        print_success "Module unloaded"
    else
        print_warning "Module not loaded"
    fi
    
    if [ -e /dev/gpioirqdev ]; then
        print_info "Removing device node..."
        sudo rm -f /dev/gpioirqdev
        print_success "Device node removed"
    fi
    echo ""
}

# Cleanup
cleanup() {
    print_title "Cleanup"
    unload_module
    
    print_info "Cleaning build files..."
    make clean > /dev/null 2>&1 || true
    
    # Remove compiled apps
    rm -f gpio_user_app gpio_test_trigger 2>/dev/null || true
    
    print_success "Cleanup complete"
    echo ""
}

# Main script
main() {
    cd "$(dirname "$0")"
    
    if [ $# -eq 0 ]; then
        # Interactive mode
        while true; do
            show_menu
            read -p "Enter choice [0-9]: " choice
            
            case $choice in
                1) full_setup ;;
                2) build_module ;;
                3) load_module ;;
                4) unload_module ;;
                5) create_device_node ;;
                6) 
                    if [ -f gpio_user_app ]; then
                        ./gpio_user_app
                    else
                        print_error "gpio_user_app not compiled. Run option 1 or 2 first."
                    fi
                    ;;
                7)
                    if [ -f gpio_test_trigger ]; then
                        ./gpio_test_trigger 10 1
                    else
                        print_error "gpio_test_trigger not compiled. Run option 1 or 2 first."
                    fi
                    ;;
                8) show_logs ;;
                9) cleanup ;;
                0) 
                    print_info "Exiting..."
                    exit 0
                    ;;
                *)
                    print_error "Invalid choice"
                    ;;
            esac
        done
    else
        # Command line mode
        case "$1" in
            setup) full_setup ;;
            build) build_module ;;
            load) load_module ;;
            unload) unload_module ;;
            device) create_device_node ;;
            compile) compile_apps ;;
            logs) show_logs ;;
            clean) cleanup ;;
            *)
                echo "Usage: $0 [setup|build|load|unload|device|compile|logs|clean]"
                exit 1
                ;;
        esac
    fi
}

main "$@"
