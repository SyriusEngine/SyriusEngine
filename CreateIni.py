def createManySpheres(n):
    file = open("./Resources/Scenes/ManySpheres.ini", "w+")
    file.write( "[ManySpheres]\n"
                "Name=\"BricksCube\"\n"
                "ObjectCount = " + str(n + 2) + "\n\n"
                "[Object0]\n"
                "Type=\"Light\"\n"
                "Name=\"Light0\"\n"
                "Color=(100.0, 100.0, 100.0)\n"
                "Position=(0.0, 3.0, 0.0)\n\n")

    file.write( "[Object1]\n"
                "Type=\"Mesh\"\n"
                "MeshType=\"Prefab\"\n"
                "PrefabType=\"Sphere\"\n"
                "Name=\"Original\"\n"
                "Position=(0.0, 0.0, 0.0)\n"
                "Scale=(1.0, 1.0, 1.0)\n"
                "Rotation=(0.0, 0.0, 0.0)\n"
                "Material=\"chipped-paint-metal\"\n")

    half_n = int(n / 2)

    IDCount = 2
    for x in range(-half_n, half_n):
        for y in range(-half_n, half_n):
            for z in range(-half_n, half_n):
                file.write( "[Object" + str(IDCount) + "]\n"
                            "Type=\"Mesh\"\n"
                            "MeshType=\"Instance\"\n"
                            "InstancedFrom=\"Original\"\n"
                            "Name=\"Sphere" + str(IDCount) + "\"\n"
                            "Position=(" + str(x * 4) + ".0, " + str(y * 4) + ".0, " + str(z * 4) + ".0)\n"
                            "Scale=(1.0, 1.0, 1.0)\n"
                            "Rotation=(0.0, 0.0, 0.0)\n"
                            "Material=\"chipped-paint-metal\"\n\n")
                IDCount += 1



def main():
    createManySpheres(6)

if __name__ == "__main__":
    main()