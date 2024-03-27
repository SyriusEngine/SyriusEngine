
def main():
    file = open("./Resources/Scenes/Slowouse.ini", "w")

    i = 1
    for x in range(5):
        for z in range(5):
            file.write("[Object" + str(i) + "]\n")
            file.write("Type=\"Mesh\"\n"
                       "Prefab=\"Cube\"\n"
                       "Scale=(1.0, 1.0, 1.0)\n"
                       "Rotation=(0.0, 0.0, 0.0)\n"
                       "Material=\"dirty-red-bricks\"\n")
            file.write("Position=(" + str(x) + ".0, 0.0, " + str(z) + ".0)\n")
            file.write("\n")
            i += 1

if __name__ == "__main__":
    main()