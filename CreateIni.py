def createFastHouse(cube_count: int = 25):
    file = open("./Resources/Scenes/FastHouse2.ini", "w+")
    width = int(cube_count ** 0.5)
    file.write("[General]\n"
               "Name=\"BricksCube\"\n"
               "ObjectCount = " + str(cube_count + 2) + "\n\n"
               "[Object0]\n"
               "Type=\"Light\"\n"
               "Name=\"Light0\"\n"
               "Color=(100.0, 100.0, 100.0)\n"
               "Position=(" + str(width / 2) + ".0, 3.0, " + str(width / 2) + ".0)\n\n")

    file.write("[Object1]\n"
                "Type=\"Mesh\"\n"
                "Prefab=\"Cube\"\n"
                "IsInstance=0\n"
                "Name=\"OriginalCube\"\n"
                "Scale=(1.0, 1.0, 1.0)\n"
                "Rotation=(0.0, 0.0, 0.0)\n"
                "Material=\"dirty-red-bricks\"\n"
                "Position=(0.0, 0.0, 0.0)\n\n")

    i = 2
    # create square of cubes
    for x in range(width):
        for z in range(width):
            file.write("[Object" + str(i) + "]\n"
                       "Type=\"Mesh\"\n"
                       "IsInstance=1\n"
                       "InstancedFrom=\"OriginalCube\"\n"
                       "Name=\"Cube" + str(i) + "\"\n"
                       "Scale=(1.0, 1.0, 1.0)\n"
                       "Rotation=(0.0, 0.0, 0.0)\n")
            file.write("Position=(" + str(x) + ".0, 0.0, " + str(z) + ".0)\n")
            file.write("\n")
            i += 1


def main():
    createFastHouse(250)

if __name__ == "__main__":
    main()