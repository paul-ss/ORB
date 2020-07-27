#include <interface.hh>

#include <iostream>


using namespace std;


class ISendData : public POA_SendData {
public:
    void putChar(::CORBA::Char msg) override;
};


void ISendData::putChar(::CORBA::Char msg) {
    cout << msg << std::flush;
}


int main(int argc, char **argv) {
    try {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
        CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
        PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

        PortableServer::Servant_var<ISendData> pSendData = new ISendData();

        PortableServer::ObjectId_var sendDataId = poa->activate_object(pSendData);


        obj = pSendData->_this();
        CORBA::String_var sior(orb->object_to_string(obj));
        cout << sior << endl;

        PortableServer::POAManager_var pman = poa->the_POAManager();
        pman->activate();


        orb->run();
    }
    catch (CORBA::SystemException &ex) {
        cerr << "Caught CORBA::" << ex._name() << endl;
    }
    catch (CORBA::Exception &ex) {
        cerr << "Caught CORBA::Exception: " << ex._name() << endl;
    }
    return 0;
}