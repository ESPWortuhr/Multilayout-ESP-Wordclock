Import("env")

uploader_flags = env.get("UPLOADERFLAGS", [])

if "--erase-all" not in uploader_flags:
    try:
        index = uploader_flags.index("write_flash")
        uploader_flags.insert(index + 1, "--erase-all")
        
        env.Replace(UPLOADERFLAGS=uploader_flags)
        print("Successfully added --erase-all to UPLOADERFLAGS")
    except ValueError:

        env.Append(UPLOADERFLAGS=["--erase-all"])
        print("Added --erase-all to the end of UPLOADERFLAGS")