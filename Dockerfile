FROM ubuntu:latest

WORKDIR /app

RUN apt-get update && apt-get install -y \
    libgtk-3-0 \
    x11-apps

COPY ./build/Image_Wicked /app/Image_Wicked

ENTRYPOINT ["./Image_Wicked"]
