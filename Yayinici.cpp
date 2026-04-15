#include "MesajPubSubTypes.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <thread>
#include <iostream>

using namespace eprosima::fastdds::dds;

int main() {
    // 1. Odadaki Adamı oluştur
    DomainParticipantQos pqos;
    pqos.name("Konusan_Adam");
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, pqos);

    // 2. Ortak Dilimizi (Mesaj.idl) tanıt
    TypeSupport type(new BenimMesajimPubSubType());
    type.register_type(participant);

    // 3. Konuşulacak Konuyu belirle
    Topic* topic = participant->create_topic("Hizli_Mesajlasma", type.get_type_name(), TOPIC_QOS_DEFAULT);

    // 4. Yayıncıyı oluştur
    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);
    DataWriter* writer = publisher->create_datawriter(topic, DATAWRITER_QOS_DEFAULT);

    BenimMesajim mesaj;
    mesaj.mesaj_numarasi() = 1;
    mesaj.icerik() = "Selam, saniyenin cok kucuk bir aninda sana ulastim!";

    std::cout << "Konusan adam basliyor..." << std::endl;

    while (true) {
        // Mesajı fırlat
        writer->write(&mesaj);
        std::cout << "Gonderildi: Mesaj No " << mesaj.mesaj_numarasi() << std::endl;
        
        mesaj.mesaj_numarasi()++;
        
        // İŞTE HIZ BURADA: Sadece 10 milisaniye bekle! (Saniyede 100 kere gönderecek)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}