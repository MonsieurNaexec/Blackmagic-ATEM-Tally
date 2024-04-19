FROM node:21-alpine

WORKDIR /usr/src/app

COPY package*.json .

RUN npm ci

COPY . .

EXPOSE 8002/udp
EXPOSE 5657/udp

CMD [ "node", "app.js" ]