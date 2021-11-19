import re
import sys


dictOfPoints = dict()
dictOfStreets = dict()

def makeTuple(t):
    t = t[1:-1]
    n = [int(i) for i in t.split(sep=',')]
    return (n[0],n[1])

def findStreets(listOfCoordinates):
    streets = []
    for ind, ite in enumerate(listOfCoordinates):
        if (ind == len(listOfCoordinates) - 1):
            break
        else:
            streets.append((ite,listOfCoordinates[ind+1]))
    return streets

def distance(a,b):
    return ((a[0] - b[0])**2 + (a[1] - b[1])**2)**0.5

def is_between(a,c,b):
    return round(distance(a,c) + distance(c,b),2) == round(distance(a,b),2)

def intersection(line1, line2):
    #y = mx+c
    f1 = False
    f2 = False
    
    if ((line1[1][0] - line1[0][0]) == 0):
        x1 = line1[0][0]
        f1 = True
    else:
        m1 = (line1[1][1] - line1[0][1]) / (line1[1][0] - line1[0][0])
        c1 = line1[1][1] - m1 * line1[1][0]
    
    if ((line2[1][0] - line2[0][0]) == 0):
        x2 = line2[0][0]
        f2 = True
    else:
        m2 = (line2[1][1] - line2[0][1]) / (line2[1][0] - line2[0][0])
        c2 = line2[1][1] - m2 * line2[1][0]
    
    if (f1 == True and f2 == True):
        return None, None, False
    elif (f1 == True and f2 == False):
        y = m2 * x1 + c2
        if (is_between(line1[0], (x1,y), line1[1]) and is_between(line2[0], (x1,y), line2[1])):
            return x1, y, True
        else:
            return None, None, False
    elif (f1 == False and f2 == True):
        y = m1 * x2 + c1
        if (is_between(line1[0], (x2,y), line1[1]) and is_between(line2[0], (x2,y), line2[1])):
            return x2, y, True
        else:
            return None, None, False
    else:
        if (m1 == m2 and not (line1[0] in line2 or line1[1] in line2)):
            return None, None, False
        elif (m1 == m2 and (line1[0] in line2 or line1[1] in line2)):
            if (line1[0] in line2):
                return line1[0][0], line1[0][1], True
            elif (line1[1] in line2):
                return line1[1][0], line1[1][1], True
        else:
            ## check the intersection (2,2)
            m = m2 - m1
            c = c1 - c2
            x = c/m
            y = m1*(x) + c1
            if (is_between(line1[0], (x,y), line1[1]) and is_between(line2[0], (x,y), line2[1])):
                return x, y, True
            else:
                return None, None, False
            
def correct(sett):
    for ind1, ite1 in enumerate(sett.copy()):
        for ind2, ite2 in enumerate(sett.copy()):
            if (ind2 > ind1):
                if(ite1[0] == ite2[1] and ite1[1] == ite2[0]):
                    sett.remove(ite1)
    return sett

def graphDetails(dictOfStreets):
    listOfStreetNames = list(dictOfStreets.keys())
    numOfStreets = len(listOfStreetNames)
    dictOfIntersections = dict()
    setOfVertices = set()
    setOfEdges = set()
    
    for i in range(numOfStreets):
        firstStreet = listOfStreetNames[i]
        for j in range(i+1, numOfStreets):
            secondStreet = listOfStreetNames[j]
            for m in dictOfStreets[firstStreet]:
                for n in dictOfStreets[secondStreet]:
                    x, y, stat = intersection(m, n)
                    if (stat == True):
                        dictOfIntersections[m, n] = (x,y)
                        setOfVertices.add(m[0])
                        setOfVertices.add(m[1])
                        setOfVertices.add(n[0])
                        setOfVertices.add(n[1])
                        setOfVertices.add((x,y))
                        if ((x,y) != m[0]):
                            setOfEdges.add(((x,y),(m[0])))
                        if ((x,y) != m[1]):
                            setOfEdges.add(((x,y),(m[1])))
                        if ((x,y) != n[0]):
                            setOfEdges.add(((x,y),(n[0])))
                        if ((x,y) != n[1]):
                            setOfEdges.add(((x,y),(n[1])))
    
    keys = list(dictOfIntersections.keys())
    for iterate in range(25):
        for k in range(len(keys)):
            intersect = dictOfIntersections[keys[k]]
            #intersect = (round(intersect[0], 2), round(intersect[0], 2))
            temp = setOfEdges.copy()
            for s in temp:
                f = False
                if (s[1][0] - s[0][0] == 0):
                    x = s[0][0]
                    f = True
                else:
                    m = (s[1][1] - s[0][1])/(s[1][0] - s[0][0])
                    c = s[1][1] - m * s[1][0]
                if (intersect == s[0] or intersect == s[1]):
                    pass
                elif (f == False and (round(intersect[1], 2) == round(m * intersect[0] + c,2)) and is_between(s[0], intersect, s[1])):
                    setOfEdges.remove(s)
                    if ((intersect, s[0]) not in setOfEdges and (s[0], intersect) not in setOfEdges):
                        setOfEdges.add((intersect, s[0]))
                    if ((intersect, s[1]) not in setOfEdges and (s[1], intersect) not in setOfEdges):
                        setOfEdges.add((intersect, s[1]))
                elif (f == True and is_between(s[0], intersect, s[1])):
                    setOfEdges.remove(s)
                    setOfEdges.add((intersect, s[0]))
                    setOfEdges.add((intersect, s[1]))
    
    return setOfVertices, correct(setOfEdges)
                
def gg(V,E):
    print(f"V {len(V)}")
    dictForEdges = dict()
    for inde, item in enumerate(V):
        dictForEdges[item] = inde + 1
    
    print("E {", end=(""))
    lE = len(E)
    for inde, item in enumerate(E):
        if (inde != lE-1):
            print(f"<{dictForEdges[item[0]]},{dictForEdges[item[1]]}>,", end=(""))
        else:
            print(f"<{dictForEdges[item[0]]},{dictForEdges[item[1]]}>", end=(""))
    print("}")





def main():
    name_str = '\s{1,}"[a-zA-Z]+(\s?[a-zA-Z]+)*"'
    coord_streets = "(\s{1,}\((-?[0]*[1-9]+[0-9]*|[0]+),(-?[0]*[1-9]+[0-9]*|[0]+)\)){2,}"
    pattern_add = 'add'+name_str + coord_streets+'$'
    pattern_mod = 'mod'+name_str + coord_streets+'$'
    pattern_rm = 'rm'+name_str+'$'
    pattern_gg = "gg"+'$'
    check_coord = re.compile(coord_streets)
    check_add = re.compile(pattern_add)
    check_mod = re.compile(pattern_mod)
    check_rm = re.compile(pattern_rm)
    check_gg = re.compile(pattern_gg)
    while (1):
        try:
            line = input()
        except EOFError:
            return
        
        if (line == ""):
            continue
        if (line == '\r' or line == '\n'):
            continue
        command = line
        

        if (len(command) != len(command.rstrip())):
            print("Error: Too many spaces after the command")
            continue
        if (len(command) != len(command.lstrip())):
            print("Error: Too many spaces before the command")
            continue
        
        
        # check command issue
        command = command.strip()
        
        
        if (len(command) == 2):
            order = command
            if (order != 'gg'):
                print("Error: Your command is erroneous, command not found.")
                continue
        elif (len(command) == 3):
            if (command in ['mod', 'add']):
                print("Error: Command (if correct) should follow other specifications such as street name, and/or coordinates.")
                continue
            else:
                print("Error: Your command is erroneous, command not found.")
                continue
        else:
            order = command[0:4]
            if (order[3] ==" "):
                order = order.strip()
                if (order not in ['add', 'mod']):
                    print("Error: Your command is erroneous, command not found or the spacing between command and street name is not considered.")
                    continue
            elif (order[3] != " " and order[2] ==" "):
                order = order[:-2].strip()
                if (order not in ['rm', 'gg']):
                    print("Error: Your command is erroneous, command not found or the spacing between command and street name is not considered.")
                    continue
            elif (order[3] =='"' and order.strip()[:-1] in ['add', 'mod']):
                print("Error: there should be enough spaces after your commands.")
                continue
            else:
                print("Error: Your command is erroneous, command not found or the spacing between command and street name is not considered.")
                continue
        if (order == 'add'):
            try:
                streetName = re.findall('"[a-zA-Z ]+"', command)[0][1:-1].lower()
            except:
                print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations and street name should be separated with only one whitespace).")
                continue
            
            if (streetName.strip() in dictOfStreets.keys()):
                print("Error: You can't add a duplicate street, if you want to modify this street use mod.")
                continue
            if (check_add.match(command) == None):
                if (len(re.findall('add[ ]{2,}', command)) != 0):
                    print("Error: Too many spaces after your command.")
                    continue
                
                if (len(re.findall('"[ ]+"', command)) != 0):
                    print("Error: Street name cannot be empty.")
                    continue
                
                if (len(streetName) != len(streetName.strip())):
                    print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations).")
                    continue
    
                if (len(re.findall("[^ ]", streetName)) == 0):
                    print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations).")
                    continue
                
                aTemp = re.findall('".*"', command)[0]
                try:
                    bTemp = re.findall('".*"( )', command)[0].strip()
                    cTemp = re.findall('( )".*"', command)[0].strip()
                except:
                    print("Error: In writing your street name, make sure there is enough space before and after.")
                    continue
                    
                if (len(streetName) == 0):
                    print("Error: Street name not defined.")
                    continue
                #error handling 1 coordinates should follow a structure
                lenOfCoordinates = len(re.findall("\(-?[0-9]+,-?[0-9]+\)", command))

                if (not check_coord.match(command)):
                    print("Error: The coordinates do not follow a legitemate shape.")
                    continue

                if (lenOfCoordinates != len(set(re.findall("\(-?[0-9]+,-?[0-9]+\)", command)))):
                    print("Error: Input is erroneous, one or more of the coordinates are repetitive.")
                    continue
                length = sum([len(q) for q in re.findall("\(-?[0-9]+,-?[0-9]+\)", command)])
    
                
                print("Error: Something was wrong with your inputs.")
                continue
            
            
            if (len(list(map(makeTuple,re.findall("\(-?[0-9]+,-?[0-9]+\)", command)))) != len(set(map(makeTuple, re.findall("\(-?[0-9]+,-?[0-9]+\)", command))))):
                print("Error: duplicate street segments.")
                continue
            elif (len(re.findall('[ ]{2,}', command)) != 0):
                print("Error: Different parts should be separted with only one space.")
                continue
            else:
                dictOfPoints[streetName] = re.findall("\(-?[0-9]+,-?[0-9]+\)", command)
                dictOfPoints[streetName] = [makeTuple(i) for i in dictOfPoints[streetName]]
                streetSegments = findStreets(dictOfPoints[streetName])
                dictOfStreets[streetName] = streetSegments
        elif (order == 'mod'):
            try:
                streetName = re.findall('"[a-zA-Z ]+"', command)[0][1:-1].lower()
            except:
                print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations with no special characters and with no spaces around).")
                continue

            if (streetName.strip() not in dictOfPoints.keys()):
                print("Error: The street name specified does not exist.")
                continue

            if (check_mod.match(command) == None):
                if (len(re.findall('mod[ ]{2,}', command)) != 0):
                    print("Error: Too many spaces after your command.")
                    continue
                
                if (len(re.findall('"[ ]+"', command)) != 0):
                    print("Error: Street name cannot be empty.")
                    continue
                    
                if (len(streetName) != len(streetName.strip())):
                    print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations with no special characters and with no spaces around).")
                    continue
    
                if (len(re.findall("[^ ]", streetName)) == 0):
                    print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations).")
                    continue
                aTemp = re.findall('".*"', command)[0]
                try:
                    bTemp = re.findall('".*"( )', command)[0].strip()
                except:
                    print("Error: In writing your street name, make sure there is enough space before and after.")
                    continue
                if (streetName not in dictOfPoints.keys()):
                    print("Error: The street name specified does not exist.")
                    continue
                
                if (len(streetName) == 0):
                    print("Error: Street name not defined.")
                    continue
                #error handling 1 coordinates should follow a structure
                lenOfCoordinates = len(re.findall("\(-?[0-9]+,-?[0-9]+\)", command))
                
                if (not check_coord.match(command)):
                    print("Error: The coordinates do not follow a legitemate shape.")
                    continue

                if (lenOfCoordinates != len(set(re.findall("\(-?[0-9]+,-?[0-9]+\)", command)))):
                    print("Error: Input is erroneous, one or more of the coordinates are repetitive.")
                length = sum([len(q) for q in re.findall("\(-?[0-9]+,-?[0-9]+\)", command)])
    

                print("Error: Something was wrong with your inputs.")
                continue
            
            if (len(list(map(makeTuple,re.findall("\(-?[0-9]+,-?[0-9]+\)", command)))) != len(set(map(makeTuple, re.findall("\(-?[0-9]+,-?[0-9]+\)", command))))):
                print("Error: duplicate street segments.")
                continue
            elif (len(re.findall('[ ]{2,}', command)) != 0):
                print("Error: Different parts should be separted with only one space.")
                continue
            else:
                dictOfPoints[streetName] = re.findall("\(-?[0-9]+,-?[0-9]+\)", command)
                dictOfPoints[streetName] = [makeTuple(i) for i in dictOfPoints[streetName]]
                streetSegments = findStreets(dictOfPoints[streetName])
                dictOfStreets[streetName] = streetSegments
        elif (order == 'rm'):
            
            try:
                streetName = re.findall('"[a-zA-Z ]+"', command)[0][1:-1].lower()
            except:
                print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations).")
                continue

            if (streetName.strip() not in dictOfPoints.keys()):
                print("Error: The street name specified does not exist.")
                continue

            if (len(command) > 2 + 1 + len(streetName) + 2):
                print("Error: rm statement doesn't follow the command format expected.")
                continue

            if (check_rm.match(command) == None):

                if (len(streetName) != len(streetName.strip())):
                    print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations).")
                    continue
                if (len(re.findall("[^ ]", streetName)) == 0):
                    print("Error: Your street name is either missing or it has not followed the structure (it should be within two qutations).")
                    continue
                
                print("Error: Something was wrong with your inputs.")
                continue
                

            if (len(re.findall('[ ]{2,}', command)) != 0):
                print("Error: Different parts should be separted with only one space.")
                continue

            del dictOfPoints[streetName]
            del dictOfStreets[streetName]
        elif (order == 'gg'):
            if (len(command)> 2):
                print("Error: gg command should not be followed by anything.")
                continue
            V, E = graphDetails(dictOfStreets)
            gg(V,E)
        
            
if __name__ == '__main__':
    main()
    sys.exit(0)
