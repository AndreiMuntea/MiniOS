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

                os.system('nasm -o "' + str(out) + '" -f bin "' + str(path) + '"')

                with open(out, 'rb') as f:
                    o.write(f.read())

                os.unlink(out)


def LoadKernel(KernelSourcePath, OutputImage):
    c_sources = ["kernel.c"]
    asm_sources = ["kernel_stub.asm"]

    lnk = ""
    obj = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "bin", "kernel.bin")

    for source in asm_sources:
        output = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "bin", source + ".o")
        src = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "asm", source)
        os.system('nasm -f elf -o "' + output + '" "' + src + '"')
        lnk += '"' + output + '" '

    for source in c_sources:
        output = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "bin", source + ".o")
        src = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "sources", source)
        os.system('i686-elf-gcc.exe -O0 -Wall -masm=intel -ffreestanding -c -o "' + output + '" "' + src + '"')
        lnk += '"' + output + '" '

    os.system('i686-elf-ld.exe -Ttext 0x110000 --oformat binary -o "' + obj + '" ' + lnk)

    with open(OutputImage, 'ab') as o:
        with open(obj, 'rb') as p:
            o.write(p.read())


def AddPadding(Path):
    floppySize = 33554432  # 32 mb
    actualSize = os.path.getsize(Path)
    padding = '\0' * (floppySize - actualSize)

    with open(Path, 'ab') as f:
        f.write(padding)


def StartBochs():
    os.system('bochsdbg.exe -q -f "' + BOSCH_CFG_FILE_PATH + '"')


if __name__ == "__main__":
    Assemblebootloader(BOOTLOADER_DIRECTORY, IMAGE_FILE_PATH)
    LoadKernel(KERNEL_SOURCE_PATH_DIRECTORY, IMAGE_FILE_PATH)
    AddPadding(IMAGE_FILE_PATH)
    StartBochs()
