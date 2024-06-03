## Introdução

Este repositório contém diversos códigos relacionados a um sistema cliente-servidor de troca de mensagens e arquivos.

## Estrutura do Repositório

O repositório está estruturado da seguinte maneira:

- `src/client/`: Código-fonte do cliente.
- `src/server/`: Código-fonte do servidor.
-  `libs/`: Wrapper para envio e recebimento de mensagens e arquivos.
- `client_data/`: Contem Músicas enviadas e recebidas pelo cliente; storage_upload/, contem as músicas utilizadas na operação de INSERT; storage_download/ contem as músicas recebidas do servidor por meio de DOWNLOAD.
- `server_data/`: Contem as Músicas e a base dados armazenada pelo servidor. As músicas estão presentes no subdiretório storage/.
- `bin/`: Contem os binários dos cliente e do servidor

## Funcionalidades

### Cliente (`client.c`)

O cliente é responsável por estabelecer uma conexão com o servidor e enviar mensagens para ele. Ele pode ser compilado e executado, aceitando dois argumentos de linha de comando: o endereço IP do servidor e a porta.

### Servidor (`server.c`)

O servidor é responsável por aguardar conexões de clientes, processar suas solicitações e responder adequadamente. Ele também gerencia um banco de dados SQLite para armazenar informações sobre músicas.

### Banco de Dados (`database.h` e `database.c`)

Esses arquivos contêm funções para gerenciar um banco de dados SQLite, incluindo a criação de tabelas e operações de inserção, seleção e exclusão de dados.

### Wrapper de Troca de Mensages (`udp_message_exchange_wrapper.h` e `udp_message_exchange_wrapper.c`)

Esses arquivos fornecem um wrapper para facilitar o envio e recebimento de mensagens entre o cliente e o servidor. Eles contêm funções para lidar com a comunicação de maneira mais conveniente.


### Wrapper de Troca de arquivos (`udp_file_exchange_wrapper.h` e `udp_file_exchange_wrapper.c`)

Funções que fornecem um wrapper para facilitar o envio e recebimento de arquivos entre o cliente e o servidor.

## Como Utilizar

1. **Instalação do SQLite3:**
   
   Antes de executar o servidor, é necessário ter o SQLite3 instalado no sistema. Para instalá-lo em sistemas baseados em Debian/Ubuntu, você pode usar o seguinte comando:
   ```
   sudo apt-get install sqlite3
   ```
   
   Em sistemas baseados em Red Hat/CentOS, você pode usar o seguinte comando:
   ```
   sudo yum install sqlite3
   ```

2. **Compilação e Execução do Servidor:**

   Compile o servidor usando o seguinte comando:
   ```
   gcc -Wall -Werror libs/* src/server/* -lsqlite3 -o bin/server
   ```

   Em seguida, execute o servidor, passando o caminho para o banco de dados SQLite como argumento. É prefirivel executar o servidor como sudo, para que não haja problemas de permissão:
   ```
   sudo ./bin/server data/music.db
   ```

3. **Compilação e Execução do Cliente:**

   Compile o cliente usando o seguinte comando:
   ```
   gcc -Wall - Werror src/client/* libs/* -o bin/client
   ```

   Em seguida, execute o cliente, passando o endereço IP e a porta do servidor como argumentos. É prefirivel executar o cliente como sudo, para que não haja problemas de permissão:
   ```
   sudo ./bin/client <endereço IP> <porta>
   ```

4. **Interagindo com o Sistema:**

   O cliente solicitará entradas do usuário para interagir com o servidor, enquanto o servidor estará aguardando por conexões.

## Observações

Certifique-se de ter permissão adequada para acessar o banco de dados SQLite e de substituir os argumentos `<endereço IP>`, `<porta>` e `database.db` pelos valores apropriados de acordo com sua configuração.