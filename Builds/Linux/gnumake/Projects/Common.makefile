# Common Include paths...
Common_Include_Path=-I"../../../../Library/Boost" \
-I"../../../../Library/Vld" \
-I"../../../../Library/Network" \
-I"../../../../Library/Database" \
-I"../../../../Library/Utility" \
-I"../../../../Library/Otl" \
-I"../../../../Library/Runtime" \
-I"../../../../Library/OpenSSL"

# Common Lib Paths
Common_Lib_Path=-L"../../../../Library/Network/gccLib" \
-L"../../../../Library/Runtime/gccLib" \
-L"../../../../Library/Utility/gccLib" \
-L"../../../../Library/Database/gccLib" 

# Common Link Libs
Common_Link_Libs_Debug=-Wl,--start-group -lRuntime -lUtility -lNetwork -lDatabase  -Wl,--end-group
Common_Link_Libs_Release=-Wl,--start-group -lRuntime -lUtility -lNetwork -lDatabase  -Wl,--end-group