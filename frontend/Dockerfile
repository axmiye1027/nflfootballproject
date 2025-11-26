FROM node:22

WORKDIR /app

COPY . .

RUN npm install

EXPOSE ${PORT}

CMD ["npm", "run", "dev"]
