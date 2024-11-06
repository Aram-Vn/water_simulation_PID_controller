import os
import subprocess
import platform


def is_fmt_installed() -> bool:
    """Check if fmt library is installed."""
    os_name: str = platform.system()

    if os_name == "Windows":
        vcpkg_path: str = os.getenv("VCPKG_ROOT", "C:/vcpkg")
        fmt_path: str = os.path.join(vcpkg_path, "installed", "x64-windows", "include", "fmt")
        
        if os.path.isdir(fmt_path):
            print("fmt library found in vcpkg directory.")
            return True
        else:
            print("fmt library not found in vcpkg directory.")
            return False

    else:
        try:
            result: subprocess.CompletedProcess = subprocess.run(
                ["pkg-config", "--exists", "fmt"],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                check=True,
            )
            return result.returncode == 0
        except (subprocess.CalledProcessError, FileNotFoundError):
            return False


def install_fmt() -> None:
    """Install fmt library depending on the operating system."""
    os_name: str = platform.system()
    print(f"Operating System detected: {os_name}")

    if os_name == "Linux":
        distro: str = ""
        try:
            with open("/etc/os-release") as f:
                for line in f:
                    if line.startswith("ID="):
                        distro = line.strip().split("=")[-1]
                        break
        except FileNotFoundError:
            pass

        if "ubuntu" in distro or "debian" in distro:
            print("Installing fmt with apt...")
            subprocess.run(["sudo", "apt", "update"])
            subprocess.run(["sudo", "apt", "install", "-y", "libfmt-dev"])
        elif "fedora" in distro or "centos" in distro:
            print("Installing fmt with dnf...")
            subprocess.run(["sudo", "dnf", "install", "-y", "fmt-devel"])
        elif "arch" in distro:
            print("Installing fmt with pacman...")
            subprocess.run(["sudo", "pacman", "-Syu", "--noconfirm", "fmt"])

    elif os_name == "Darwin":  # macOS
        print("Installing fmt with Homebrew...")
        subprocess.run(["brew", "install", "fmt"])

    elif os_name == "Windows":
        print("Please use vcpkg to install fmt on Windows:")
        print("1. Download and install vcpkg (https://github.com/microsoft/vcpkg)")
        print("2. Run 'vcpkg install fmt'")
    else:
        print("Unsupported OS. Please install fmt manually.")


def main() -> None:
    if is_fmt_installed():
        print("fmt library is already installed.")
    else:
        print("fmt library not found. Installing...")
        install_fmt()


if __name__ == "__main__":
    main()
