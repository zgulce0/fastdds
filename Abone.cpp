#include "MesajPubSubTypes.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <iostream>

using namespace eprosima::fastdds::dds;

// Kulağı kapıda olan dinleyici
class Dinleyici : public DataReaderListener {
public:
    void on_data_available(DataReader* reader) override {
        BenimMesajim mesaj;
        SampleInfo info;
        // Eğer okunacak bir şey varsa al
        if (reader->take_next_sample(&mesaj, &info) == ReturnCode_t::RETCODE_OK) {
            if (info.valid_data) {
                std::cout << "ALINDI -> No: " << mesaj.mesaj_numarasi() 
                          << " | İcerik: " << mesaj.icerik() << std::endl;
            }
        }
    }
};

int main() {
    DomainParticipantQos pqos;
    pqos.name("Dinleyen_Adam");
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, pqos);

    TypeSupport type(new BenimMesajimPubSubType());
    type.register_type(participant);

    Topic* topic = participant->create_topic("Hizli_Mesajlasma", type.get_type_name(), TOPIC_QOS_DEFAULT);

    Subscriber* subscriber = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
    
    Dinleyici listener;
    subscriber->create_datareader(topic, DATAREADER_QOS_DEFAULT, &listener);

    std::cout << "Dinleyen Adam sessizce bekliyor..." << std::endl;
    
    // Sonsuza kadar dinlemeye devam et
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
