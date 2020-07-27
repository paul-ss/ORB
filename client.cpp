#include <interface.hh>

#include <iostream>
#include <cstdio>
#include <termios.h>

#define EXIT_FAILURE 1
#define CTRLD 4   // Ctrl + d doesn't work in noncanonical mode


using namespace std;


void setNoncanonicalMode() {
    termios newSettings = {};
    tcgetattr(0, &newSettings);

    newSettings.c_lflag &= ~ICANON;
    newSettings.c_cc[VTIME] = 0;
    newSettings.c_cc[VMIN] = 1;

    tcsetattr(0, TCSANOW, &newSettings);
}


void readCharLoop(const SendData_var &srvRef) {
    while (true) {
        char ch = getchar();
        if (ch == CTRLD) {
            cout << endl;
            break;
        }

        srvRef->putChar(ch);
    }
}


int main(int argc, char **argv) {
    try {
        if (argc != 2) {
            cerr << "usage:  eg2_clt <object reference>" << endl;
            return EXIT_FAILURE;
        }

        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
        CORBA::Object_var obj = orb->string_to_object(argv[1]);
        SendData_var srvRef = SendData::_narrow(obj);

        if (CORBA::is_nil(srvRef)) {
            cerr << "Can't narrow reference to type Echo (or it was nil)." << endl;
            return EXIT_FAILURE;
        }

        setNoncanonicalMode();
        readCharLoop(srvRef);

        orb->destroy();
    }
    catch (CORBA::TRANSIENT &) {
        cerr << "Caught system exception TRANSIENT -- unable to contact the "
             << "server." << endl;
    }
    catch (CORBA::SystemException &ex) {
        cerr << "Caught a CORBA::" << ex._name() << endl;
    }
    catch (CORBA::Exception &ex) {
        cerr << "Caught CORBA::Exception: " << ex._name() << endl;
    }
    return 0;
}

