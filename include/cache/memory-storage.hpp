#pragma once
//Temporary cache storing files from fs, collected from remote sources or pre-compiled.
//This is not the final cache based on sqlite, but entries might be promoted under some conditions.
//This cache exists regardless of the other one being enabled or not, and has a functional purpose.

//Once the build process of an app is completed, this cache is fully released before running.
//This cache is going to be global.