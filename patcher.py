from os import listdir,system

patches = listdir("./patches")


for index,patch in enumerate(patches):
    print(f"[{index+1}]- {patch}")

try:
    patch_id = int(input("\nchooce patche id: "))
except ValueError:
    print("Not a valid number")
    exit(1)


if patch_id <1 or patch_id > len(patches):
    print("Out of range!!")
    exit(1)



if patches[patch_id-1] == "slstatus":
    print("")
    system("cp -r patches/slstatus/components .")
    system("patch < patches/slstatus/slstatus.patch")
    print("")
    print("don't forget to run:\n\t sudo make install")
else:
    print("Out of range!!")
    exit(1)


