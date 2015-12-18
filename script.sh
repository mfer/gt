wget -c https://ccl.northwestern.edu/netlogo/5.2.0/netlogo-5.2.0.tar.gz

tar -xzf netlogo-5.2.0.tar.gz

cd netlogo-5.2.0/models
wget -c http://ccl.northwestern.edu/netlogo/community/GameTheory.nlogo
cd ..
./netlogo.sh models/GameTheory.nlogo



#./netlogo.sh models/Curricular\ Models/Urban\ Suite/Urban\ Suite\ -\ Tijuana\ Bordertowns.nlogo
