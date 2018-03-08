project (decryptor)
add_executable(decryptor 
    main.cpp
    interface.cpp
    interface.h)
target_link_libraries(decryptor cardanus)