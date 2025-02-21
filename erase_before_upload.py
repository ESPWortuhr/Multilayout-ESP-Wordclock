Import("env")

old_uploaderflags = env["UPLOADERFLAGS"]
#print("Old uploaderflags: " + str(old_uploaderflags))
    
index_write_flash = old_uploaderflags.index("write_flash")
if index_write_flash != -1: 
    new_uploaderflags = old_uploaderflags[::]
    new_uploaderflags.insert(index_write_flash + 1, "--erase-all")
    #print("Replaced with: " + str(new_uploaderflags))
    env.Replace(UPLOADERFLAGS=new_uploaderflags)