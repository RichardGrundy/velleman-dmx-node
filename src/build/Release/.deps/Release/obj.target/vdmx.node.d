cmd_Release/obj.target/vdmx.node := g++ -shared -pthread -rdynamic -m64  -Wl,-soname=vdmx.node -o Release/obj.target/vdmx.node -Wl,--start-group Release/obj.target/vdmx/velleman-node-dmx.o -Wl,--end-group 