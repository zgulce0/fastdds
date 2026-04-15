# 1. Tertemiz bir Ubuntu bilgisayarı alıyoruz
FROM ubuntu:22.04

# Kurulum sırasında bize soru sormasını engelliyoruz
ENV DEBIAN_FRONTEND=noninteractive

# 2. Gerekli alet çantasını kuruyoruz (C++, CMake, Git, Java vb.)
RUN apt-get update && apt-get install -y \
    cmake g++ git build-essential \
    libasio-dev libtinyxml2-dev libssl-dev default-jdk wget

WORKDIR /kurulum

# 3. Fast CDR'ı Kuruyoruz (Veri paketleme kütüphanesi)
RUN git clone https://github.com/eProsima/Fast-CDR.git \
    && cd Fast-CDR && mkdir build && cd build \
    && cmake .. && make -j4 && make install

# 4. Fast DDS'i Kuruyoruz (Ana haberleşme kütüphanesi)
RUN git clone https://github.com/eProsima/Fast-DDS.git \
    && cd Fast-DDS && mkdir build && cd build \
    && cmake -DTHIRDPARTY=ON -DCOMPILE_EXAMPLES=OFF .. \
    && make -j4 && make install

# 5. Fast DDS Gen'i Kuruyoruz (Ortak dil olan IDL dosyasından C++ kodu üreten araç)
RUN git clone --recursive https://github.com/eProsima/Fast-DDS-Gen.git \
    && cd Fast-DDS-Gen \
    && ./gradlew assemble \
    && cp scripts/fastddsgen /usr/local/bin/ \
    && mkdir -p /usr/local/share/fastddsgen/java/ \
    && cp share/fastddsgen/java/fastddsgen.jar /usr/local/share/fastddsgen/java/

# Kütüphanelerin yerini sisteme tanıtıyoruz
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# 6. Şimdi kendi projemize geçiyoruz
WORKDIR /app
COPY . /app

# 7. Ustaya kodları derleyip çalıştırılabilir hale getirmesini söylüyoruz
RUN cmake . && make