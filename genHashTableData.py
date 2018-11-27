
import random

targetOps = 2000000

outFile = open("InsRem-50-64K.dat", "w")

valueMax = 64000
targetIns = int(float(targetOps) * 0.5)
targetRem = int(float(targetOps) * 0.5)

numIns = 0
numRem = 0

values = []

for i in range(0, targetOps):
    opType = random.randint(0, 1)

    if i < int(float(targetOps) * 0.25):
        opType = 0
        
    if opType == 0 and targetIns == numIns:
        opType = 1
    elif opType == 1 and targetRem == numRem:
        opType = 0

    if opType == 1 and len(values) == 0:
        opType = 0

    if opType == 0: 
        value = random.randint(1, valueMax)
        outFile.write("INSERT " + str(value) + "\n")
        numIns += 1
        if value not in values:
            values.append(value)
    elif opType == 1:
        valueIdx = random.randint(0, len(values) - 1)
        value = values[valueIdx]
        values.pop(valueIdx)
        outFile.write("REMOVE " + str(value) + "\n")
        numRem += 1
    else:
        outFile.write("ERROR opType = " + str(opType) + "\n")
    
outFile.close()

outFile = open("Contains-90-64K.dat", "w")

valueMax = 64000
targetCon = int(float(targetOps) * 0.9)
targetIns = int(float(targetOps) * 0.09)
targetRem = int(float(targetOps) * 0.01)

numCon = 0
numIns = 0
numRem = 0

values = []

for i in range(0, targetOps):
    opType = random.randint(0, 99)
    
    if opType < 70:
        opType = 2
    elif opType < 90:
        opType = 0
    else:
        opType = 1
        
    if i < int(float(targetOps) * 0.05):
        opType = 0

    if opType == 2 and targetCon == numCon:
        opType = random.randint(0,1)

    if opType == 0 and targetIns == numIns:
        opType = 1
    elif opType == 1 and targetRem == numRem:
        opType = 0
        
    if targetIns == numIns and targetRem == numRem:
        opType = 2

    if opType == 1 and len(values) == 0:
        opType = 0

    if opType == 0: 
        value = random.randint(0, valueMax)
        outFile.write("INSERT " + str(value) + "\n")
        numIns += 1
        if value not in values:
            values.append(value)
    elif opType == 1:
        valueIdx = random.randint(0, len(values) - 1)
        value = values[valueIdx]
        values.pop(valueIdx)
        outFile.write("REMOVE " + str(value) + "\n")
        numRem += 1
    elif opType == 2:
        outFile.write("CONTAINS " + str(random.randint(0, valueMax)) + "\n")
        numCon += 1
    else:
        outFile.write("ERROR opType = " + str(opType) + "\n")
    
outFile.close()

outFile = open("InsRem-50-256.dat", "w")

valueMax = 256
targetIns = int(float(targetOps) * 0.5)
targetRem = int(float(targetOps) * 0.5)

numIns = 0
numRem = 0

values = []

for i in range(0, targetOps):
    opType = random.randint(0, 1)

    if i < int(float(targetOps) * 0.25):
        opType = 0
        
    if opType == 0 and targetIns == numIns:
        opType = 1
    elif opType == 1 and targetRem == numRem:
        opType = 0

    if opType == 1 and len(values) == 0:
        opType = 0

    if opType == 0: 
        value = random.randint(1, valueMax)
        outFile.write("INSERT " + str(value) + "\n")
        numIns += 1
        if value not in values:
            values.append(value)
    elif opType == 1:
        valueIdx = random.randint(0, len(values) - 1)
        value = values[valueIdx]
        values.pop(valueIdx)
        outFile.write("REMOVE " + str(value) + "\n")
        numRem += 1
    else:
        outFile.write("ERROR opType = " + str(opType) + "\n")
    
outFile.close()

outFile = open("Contains-90-256.dat", "w")

valueMax = 256
targetCon = int(float(targetOps) * 0.9)
targetIns = int(float(targetOps) * 0.09)
targetRem = int(float(targetOps) * 0.01)

numCon = 0
numIns = 0
numRem = 0

values = []

for i in range(0, targetOps):
    opType = random.randint(0, 99)
    
    if opType < 70:
        opType = 2
    elif opType < 90:
        opType = 0
    else:
        opType = 1
        
    if i < int(float(targetOps) * 0.05):
        opType = 0

    if opType == 2 and targetCon == numCon:
        opType = random.randint(0,1)

    if opType == 0 and targetIns == numIns:
        opType = 1
    elif opType == 1 and targetRem == numRem:
        opType = 0
        
    if targetIns == numIns and targetRem == numRem:
        opType = 2

    if opType == 1 and len(values) == 0:
        opType = 0

    if opType == 0: 
        value = random.randint(0, valueMax)
        outFile.write("INSERT " + str(value) + "\n")
        numIns += 1
        if value not in values:
            values.append(value)
    elif opType == 1:
        valueIdx = random.randint(0, len(values) - 1)
        value = values[valueIdx]
        values.pop(valueIdx)
        outFile.write("REMOVE " + str(value) + "\n")
        numRem += 1
    elif opType == 2:
        outFile.write("CONTAINS " + str(random.randint(0, valueMax)) + "\n")
        numCon += 1
    else:
        outFile.write("ERROR opType = " + str(opType) + "\n")
    
outFile.close()