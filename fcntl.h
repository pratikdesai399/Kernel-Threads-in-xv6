#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200

//Clone Flags
#define CLONE_VM 1     //Set if VM shared 
#define CLONE_FS 2    //Set if fs info shared
#define CLONE_FILES 4 //Set if open files shared
#define CLONE_PARENT 8 //Set if the threads wants same parent as cloning process
#define CLONE_THREAD 16 //Same Thread Group
