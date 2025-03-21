id=7
for i in range(256, 352):
    print(f"<io id=\"{id}\"	name=\"Out{i}\"		db=\"%QX0.{i}\"	type=\"BOOL\"		description=\"In{i} digital output\"/>") 
    id+=1