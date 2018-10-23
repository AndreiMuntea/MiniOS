import os


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

                path    = '"' + os.path.join(root, f) + '"'
                obj     = os.path.join(root, f) + '.obj'
                out     = '"' + obj + '"'

                os.system('nasm -O0 -o ' + out + ' -f bin ' + path)
                with open(obj, 'rb') as ob:
                    o.write(ob.read())
                
                os.unlink(obj)


def LoadKernel(KernelSourcePath, OutputImage):
    bin_directory   = os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "bin")
    obj             = os.path.join(bin_directory, "kernel.bin")
    lnk             = ""

    for root, _, files in os.walk(KERNEL_SOURCE_PATH_DIRECTORY):
        for f in files:
            src     = '"' + os.path.join(root, f) + '"'
            output  = '"' + os.path.join(bin_directory, f + '.obj') + '"'
            asmo    = '"' + os.path.join(bin_directory, f + '.asmo') + '"'
            headers = '"' + os.path.join(KERNEL_SOURCE_PATH_DIRECTORY, "headers") + '"'

            if f.endswith(".asm"):
                os.system('nasm -f elf64 -O0 -o ' + output + ' ' + src)
            elif f.endswith(".c"):
                os.system('cc1_x86_x64.exe -mabi=ms -std=c99 -ffreestanding -m64 -O0 ' + src + ' -o ' + asmo + ' -Wall -masm=intel -I ' + headers)
                os.system('as_x86_x64.exe --64 "' + asmo + '" -o "' + output + '" -msyntax=intel')
            else:
                continue
            lnk += ' "' + output + '" ' 

    os.system('ld_x86_x64.exe -O0 -Ttext 0x110000 -Tdata 0x125000 -Tbss 0x150000 --oformat binary -o ' + obj + ' ' + lnk + ' -m elf_x86_64')
    
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
