import os
import tempfile

BOOTLOADER_DIRECTORY = os.path.join(os.getcwd(), "bootloader")
IMAGE_FILE_PATH = os.path.join(os.getcwd(), "bin", "image.bin")
KERNEL_SOURCE_PATH_DIRECTORY = os.path.join(os.getcwd(), "kernel")
BOSCH_CFG_FILE_PATH = os.path.join(os.getcwd(), "bochs", "mini-os.bxrc")


def Assemblebootloader(AsmInputDirectory, OutputImage):
    with open(OutputImage, 'wb') as o:
        for root, _, files in os.walk(AsmInputDirectory):
            for f in files:
                if not f.endswith(".asm"):
                    continue
                path = os.path.join(root, f)
                out = path + ".obj"

                os.system('nasm -O0 -o "' + str(out) + '" -f bin "' + str(path) + '"')

                with open(out, 'rb') as f:
                    o.write(f.read())

                os.unlink(out)


def LoadKernel(KernelSourcePath, OutputImage):
    c_sources = ["kernel.c", "global.c", "screen.c"]
    asm_sources = ["kernel_stub.asm", "commons.asm"]

    lnk = ""
    obj = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "bin", "kernel.bin")
    headers = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "headers")

    for source in asm_sources:
        src = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "asm", source)
        output = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "bin", source + ".o")

        os.system('nasm -f elf64 -O0 -o "' + output + '" "' + src + '"')

        lnk += '"' + output + '" '

    for source in c_sources:
        src = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "sources", source)
        asmo = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "bin", source + ".asmo")
        output = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "bin", source + ".obj")

        os.system('cc1_x86_x64.exe -std=c99 -ffreestanding -m64 -O0 "' + src + '" -o "' + asmo + '" -Wall -masm=intel -I "' + headers +'"')
        os.system('as_x86_x64.exe --64 "' + asmo + '" -o "' + output + '" -msyntax=intel')

        lnk += '"' + output + '" '

    os.system('ld_x86_x64.exe -O0 -Ttext 0x110000 -Tdata 0x125000 -Tbss 0x150000 --oformat binary -o "' + obj + '" ' + lnk + " -m elf_x86_64 ")

    with open(OutputImage, 'ab') as o:
        with open(obj, 'rb') as p:
            o.write(p.read())


def AddPadding(Path):
    diskSize = 33554432  # 32 mb
    actualSize = os.path.getsize(Path)
    padding = '\0' * (diskSize - actualSize)

    with open(Path, 'ab') as f:
        f.write(padding)


def StartBochs():
    os.system('bochsdbg.exe -q -f "' + BOSCH_CFG_FILE_PATH + '"')


if __name__ == "__main__":
    Assemblebootloader(BOOTLOADER_DIRECTORY, IMAGE_FILE_PATH)
    LoadKernel(KERNEL_SOURCE_PATH_DIRECTORY, IMAGE_FILE_PATH)
    AddPadding(IMAGE_FILE_PATH)
    StartBochs()
