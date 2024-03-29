linux_installation_guide: 

1. For Ubuntu systems (currently just test for Ubuntu16.04)
    1). install denpendancy packages
            sudo apt-get update
            sudo apt-get install cmake git libreadline-dev uuid-dev g++ libncurses5-dev zip libssl-dev openssl build-essential python-dev pkg-config autoconf autotools-dev libicu-dev libbz2-dev libboost-dev libboost-all-dev
            export LC_ALL="en_US.UTF-8"

    2).  install ntp time and do configurations

            sudo apt-get install ntp
            sudo apt-get install ntpdate
            sudo service ntp stop
            sudo ntpdate -s time.nist.gov
            sudo service ntp start

    3). install the leveldb [1.18 or later]
            download leveldb version 1.20 [https://github.com/google/leveldb/releases]
            
            wget https://github.com/google/leveldb/archive/v1.20.tar.gz
            tar -zxvf v1.20.tar.gz
            cd leveldb-1.20/
            make
            sudo scp out-static/lib*  /usr/local/lib/ 
            sudo ldconfig

    4). install the miniupnpc [ 1.8 ]
            download http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.8.20131209.tar.gz
            
            wget -O miniupnpc-1.8.20131209.tar.gz http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.8.20131209.tar.gz
            tar -zxvf miniupnpc-1.8.20131209.tar.gz
            cmake .
            make
            To install the library and headers on the system use :
            > su make install
            > exit
             alternatively, to install into a specific location, use :
            > INSTALLPREFIX=/usr/local make install

     5). install fast compile library
            git clone https://github.com/tongtongchain/bitshares-fc
            cd fast-compile
            git checkout static_variant_string_tag
            git submodule update --init --recursive
            
            cmake .
            make
            sudo cp libfc.a  /usr/local/lib/
            sudo cp vendor/secp256k1-zkp/src/project_secp256k1-build/.libs/libsecp256k1.a /usr/local/lib
            
     6). build BLOCKCHAIN code
                cd Chain/
                cmake . [ -DBOOST_ROOT=xxxx  -DOPENSSL_ROOT_DIR=xxxx]
                make
                
            NOTE : if you install boost and openssl into some other directory, you need change the CMakeList.txt,
            whereabouts the option ${centos}
                
2. For Centos 7.2/ 7.3.1611 systems (currently just test for Centos 7.2 & 7.3.1611)
    GCC 4.8.5
    pre-installation:
     install denpendancy packages
             sudo yum -y install cmake git readline-devel uuid-devel g++ ncurses-devel zip openssl openssl-devel openssl-static build-essential pkgconfig python-dev autoconf autotools-devel libicu-devel libbz2-devel
             export LC_ALL="en_US.UTF-8"
             
     NOTE： Manually install Boost 1.59 and openssl 1.0.2k into default /usr/local directory
     1).  install boost 1.59 
           wget http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz
           tar -zxvf boost_1_59_0.tar.gz
           cd boost_1_59_0
           ./bootstrap.sh  
           ./b2
           ./b2 install
           
     2). install openssl 1.0.2k
           wget https://www.openssl.org/source/old/1.0.2/openssl-1.0.2k.tar.gz
           tar -zxvf openssl-1.0.2k.tar.gz
           cd openssl-1.0.2k
           ./config
           make
           make install

     3). install ntp time and do configurations

     4). install the leveldb [1.18 or later]
            download leveldb version 1.20 [https://github.com/google/leveldb/releases]
            
            wget https://github.com/google/leveldb/archive/v1.20.tar.gz
            tar -zxvf v1.20.tar.gz
            cd leveldb-1.20/
            make
            sudo scp out-static/lib*  /usr/local/lib/        
            sudo ldconfig

    5). install the miniupnpc [ 1.8 ]
            download http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.8.20131209.tar.gz
            
            wget -O miniupnpc-1.8.20131209.tar.gz http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.8.20131209.tar.gz
            tar -zxvf miniupnpc-1.8.20131209.tar.gz
            cmake .
            make
            To install the library and headers on the system use :
            > su make install
            > exit
             alternatively, to install into a specific location, use :
            > INSTALLPREFIX=/usr/local make install

     6). install fast compile library
            git clone https://github.com/tongtongchain/bitshares-fc
            cd fast-compile
            git checkout static_variant_string_tag
            git submodule update --init --recursive
            
            cmake .
            make
            sudo cp libfc.a  /usr/local/lib/
            sudo cp vendor/secp256k1-zkp/src/project_secp256k1-build/.libs/libsecp256k1.a /usr/local/lib
            
     7). build BLOCKCHAIN code
                cd Chain/
                cmake . [ -DBOOST_ROOT=xxxx  -DOPENSSL_ROOT_DIR=xxxx]
                make
                
       NOTE : if you install boost and openssl into some other directory, you need change the CMakeList.txt,
                    whereabouts the option ${centos}
               
3. For Fedora 27/25 systems 
    pre-installation:
     install denpendancy packages
             dnf install automake
             dnf install cmake  git  libtool readline-devel uuid-devel gcc-c++ ncurses-devel zip  pkgconfig python-devel autoconf libicu-devel  bzip2-devel
             dnf install compat-openssl10 
             
     NOTE： Manually install Boost 1.59 and openssl 1.0.2k into default /usr/local directory. 
     NOTE:    NOT USE OPENSSL 1.1 VERSION and BOOST version higher 1.60
     
     1).  install boost 1.59 
           wget http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz
           tar -zxvf boost_1_59_0.tar.gz
           cd boost_1_59_0
           ./bootstrap.sh  
           ./b2
           ./b2 install
           
     2). install openssl 1.0.2k
           wget https://www.openssl.org/source/old/1.0.2/openssl-1.0.2k.tar.gz
           tar -zxvf openssl-1.0.2k.tar.gz
           cd openssl-1.0.2k
           ./config --prefix=/usr/local
           make
           make install

     3). install ntp time and do configurations

     4). install the leveldb [1.18 or later]
            download leveldb version 1.20 [https://github.com/google/leveldb/releases]
            
            wget https://github.com/google/leveldb/archive/v1.20.tar.gz
            tar -zxvf v1.20.tar.gz
            cd leveldb-1.20/
            make
            sudo scp out-static/lib*  /usr/local/lib/        
            sudo ldconfig

    5). install the miniupnpc [ 1.8 ]
            download http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.8.20131209.tar.gz
            
            wget -O miniupnpc-1.8.20131209.tar.gz http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.8.20131209.tar.gz
            tar -zxvf miniupnpc-1.8.20131209.tar.gz
            cmake .
            make
            To install the library and headers on the system use :
            > su make install
            > exit
             alternatively, to install into a specific location, use :
            > INSTALLPREFIX=/usr/local make install

     6). install fast compile library
            git clone https://github.com/tongtongchain/bitshares-fc
            cd fast-compile
            git checkout static_variant_string_tag
            git submodule update --init --recursive
            
            cmake . -DOPENSSL_ROOT_DIR=(openssl root directory)
            make
            sudo cp libfc.a  /usr/local/lib/
            sudo cp vendor/secp256k1-zkp/src/project_secp256k1-build/.libs/libsecp256k1.a /usr/local/lib
            
     7). build BLOCKCHAIN code
                cd Chain/
                cmake . [ -DBOOST_ROOT=xxxx  -DOPENSSL_ROOT_DIR=xxxx]
                make
              
                
