# Banco JC

[![CodeFactor](https://www.codefactor.io/repository/github/jpereira1330/banco/badge/master)](https://www.codefactor.io/repository/github/jpereira1330/banco/overview/master)

Banco JC se trata de um projeto para exemplificar uma conexão entre cliente e servidor desenvolvida em C++, as operações realizadas pelo banco são as seguintes:

  - Saques
  - Depositos
  - Transferencia
  - Criar contas
  - Conexão com mais de uma conta simultaneamente.

# Fluxo de comunicação

  - Cliente se conecta no servidor utlizando uma porta passada como parametro de execução
  - Cliente solicita ao usuario informar o numero da conta e o envia para o servidor
  - Servidor valido se o numero da conta existe, caso não, cliente solicita um 

## Significado de cada tipo

| Tipo | Descrição | Origem | Destino |
| ------ | ------ | ------ | ------ | 
| C | Verifique a seguinte conta existe | Cliente | Servidor |
| C | A conta informada existe | Servidor | Cliente |
| c | A conta informada não existe | Servidor | Cliente |
| N | Criar uma nova conta | Cliente | Servidor |
| N | Conta criada com sucesso | Servidor | Cliente |
| n | Não foi possivel criar uma nova conta | Servidor | Cliente |
| S | Sacar dinheiro na conta | Cliente | Servidor |
| S | Dinheiro sacado com sucesso | Servidor | Cliente |
| s | Não foi possivel Sacar Dinheiro | Servidor | Cliente |
| D | Depositar dinheiro na conta | Cliente | Servidor |
| D | Dinheiro depositado com sucesso | Servidor | Cliente |
| d | Não foi possivel depositar dinheiro na conta | Servidor | Cliente |
| T | Transferir dinheiro para outra conta | Cliente | Servidor |
| T | Dinheiro Transferido com sucesso | Servidor | Cliente |
| t | Não foi possivel Transfererir dinheiro | Servidor | Cliente |
    
### Como utilizar

Banco JC requer [compilador g++](https://nodejs.org/) v5.4+ para compilar

Como instalar as dependencias para a correta utilização do software

```sh
$ sudo apt install g++ build-essential -y
```

Para compilar os projetos você possui duas opções, executando o arquivo presente no diretorio do projeto
```sh
$ ./compilador
```
ou, executando os eginte comando:
```sh
$ g++ *.h *.c *.cpp -l pthread -std=c++11 -o banco
```

Para a execução é de necessidade informar alguns parametros, onde oque esta em conchetes deverá ser subistituido pelas informações.

**Caso Cliente:**
```sh
$ ./banco [IP] [PORTA]
```
**Caso Servidor:**
```sh
$ ./banco [PORTA]
```
