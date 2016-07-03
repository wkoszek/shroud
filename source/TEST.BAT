ls -cq *.t *.h | xargs "shroud -eINCLUDE \temp"
copy * \temp
cd \temp
mv test.t test.c
cl *.c