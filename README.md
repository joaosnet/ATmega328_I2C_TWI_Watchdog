# ATmega328_I2C_TWI_Watchdog - Contador LCD com reset de 2 segundos - ATmega328

![GitHub repo size](https://img.shields.io/github/repo-size/joaosnet/ATmega328_I2C_TWI_Watchdog?style=for-the-badge)
![GitHub language count](https://img.shields.io/github/languages/count/joaosnet/ATmega328_I2C_TWI_Watchdog?style=for-the-badge)
![GitHub forks](https://img.shields.io/github/forks/joaosnet/ATmega328_I2C_TWI_Watchdog?style=for-the-badge)
![Bitbucket open issues](https://img.shields.io/bitbucket/issues/joaosnet/ATmega328_I2C_TWI_Watchdog?style=for-the-badge)
![Bitbucket open pull requests](https://img.shields.io/bitbucket/pr-raw/joaosnet/ATmega328_I2C_TWI_Watchdog?style=for-the-badge)

<img src="screenshots\projeto_proteus.bmp" alt="Projeto Proteus">

> Controle de um microcontrolador ATmega328 com comunicação I2C e Watchdog Timer. O programa realiza a inicialização do I2C, inicia a comunicação I2C, escreve em um display LCD e configura o Watchdog Timer para reiniciar o microcontrolador em caso de falhas.

### Ajustes e melhorias

O projeto está em andamento, e as próximas atualizações serão focadas nas seguintes tarefas:

1. [x] **Código do Controlador Funcionando:**
   - Ajustes finais no código para garantir que o controlador funcione corretamente conforme os requisitos do projeto.
   - Testes e depuração para verificar o funcionamento adequado.

2. [x] **Artigo sobre o Projeto:**
   - Elaboração de um artigo seguindo o modelo de artigo empregado pelo SBrT, com pelo menos quatro páginas.
   - Inclusão de detalhes sobre o circuito, código, teoria envolvida e memória de cálculos realizados.
   - Referências apropriadas para todo o material utilizado, evitando reprodução integral de conteúdos de terceiros.

3. [x] **Apresentação do Projeto:**
   - Preparação de slides para apresentação, incluindo detalhes da teoria, desenvolvimento do projeto e resultados.
   - Demonstração do projeto funcionando, seja em simulação ou montagem física, durante a apresentação.

4. [x] **Envio do Trabalho:**
   - Compilação de todos os documentos relacionados ao projeto (trabalho escrito, código-fonte, slides de apresentação) em um arquivo compactado (.zip).
   - Nome do arquivo: `2023.4_3.zip`.

## 💻 Pré-requisitos

Antes de começar, verifique se você atendeu aos seguintes requisitos:

- Você instalou a versão mais recente do Microchip Studio
- Você tem instalado o Proteus 8.7 SP3 ou um microcontrolador ATmega328p
- Você leu a [o artigo relacionado ao projeto](https://github.com/joaosnet/ATmega328_I2C_TWI_Watchdog/blob/67f15c2138273b703f4898307c57c47d2b9cf023/docs/Explorando_o_ATmega328_com_I2C_e_Watchdog_em_C_Puro.pdf).

## ☕ Usando Contador Sete Segmentos

Abrar o projeto no Microchip Studio e faça o build do projeto. Após isso, abra o Proteus e carregue o arquivo `ATmega328_I2C_TWI_Watchdog.pdsprj` e execute a simulação.

Adicione comandos de execução e exemplos que você acha que os usuários acharão úteis. Fornece uma referência de opções para pontos de bônus!

## 🤝 Colaboradores

Agradecemos às seguintes pessoas que contribuíram para este projeto:

<table>
  <tr>
    <td align="center">
      <a href="https://www.instagram.com/jaonativi/" title="Gerente de Projetos Desenvolvedor Backend">
        <img src="https://avatars.githubusercontent.com/u/87316339?v=4" width="100px;" alt="Foto do João Natividade no GitHub"/><br>
        <sub>
          <b>João Natividade</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://www.instagram.com/aly_lustoza/" title="QA Tester Analista de Mercado">
        <img src="https://instagram.fbel1-1.fna.fbcdn.net/v/t51.2885-19/274501676_537691970910933_7250418063848294931_n.jpg?stp=dst-jpg_s150x150&_nc_ht=instagram.fbel1-1.fna.fbcdn.net&_nc_cat=109&_nc_ohc=KgbJtfViehAAX9mB9cR&edm=ACWDqb8BAAAA&ccb=7-5&oh=00_AfApyka_2lTirZVLEmvZec5PMm-mFW-SU1m3uIYbYDNZ5w&oe=65664ACE&_nc_sid=ee9879" width="100px;" alt="Foto do Alydson Lustoza"/><br>
        <sub>
          <b>Alydson Lustoza</b>
        </sub>
      </a>
        <td align="center">
      <a href="https://www.instagram.com/c.j_silva/" title="UX Designer Desenvolvedora Backend">
        <img src="https://instagram.fbel1-1.fna.fbcdn.net/v/t51.2885-19/361771304_833908247939807_9019221283482484802_n.jpg?stp=dst-jpg_s150x150&_nc_ht=instagram.fbel1-1.fna.fbcdn.net&_nc_cat=111&_nc_ohc=6VEjSCPRkm0AX-I87Do&edm=ACWDqb8BAAAA&ccb=7-5&oh=00_AfAZPuIQzYFL-7ojBTLjwU-cWWeeBRhDd_4y8QxFkIYUqw&oe=656549F2&_nc_sid=ee9879" width="100px;" alt="Foto da Carla Silva"/><br>
        <sub>
          <b>Carla Silva</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://www.instagram.com/tms.jpeg/" title="UI Designer Especialista em Marketing">
        <img src="https://instagram.fbel1-1.fna.fbcdn.net/v/t51.2885-19/300225876_600418448208671_8750170704882872093_n.jpg?stp=dst-jpg_s150x150&_nc_ht=instagram.fbel1-1.fna.fbcdn.net&_nc_cat=111&_nc_ohc=zkwa2sgjdMYAX-g_-gi&edm=ACWDqb8BAAAA&ccb=7-5&oh=00_AfCdAENfZaw2HUE5i5Mj9XrHStvDv2d348cJUBAutNyrlg&oe=6566888B&_nc_sid=ee9879" width="100px;" alt="Foto do José Soares"/><br>
        <sub>
          <b>José Soares</b>
        </sub>
      </a>
    </td>
<td align="center">
      <a href="https://www.linkedin.com/in/haroldo-magano-j%C3%BAnior-1a0b2265/" title="UI Designer Especialista em Marketing">
        <img src="https://media.licdn.com/dms/image/C5635AQF8PVFSmAHKyw/profile-framedphoto-shrink_400_400/0/1615418813549?e=1701450000&v=beta&t=HeCNpYmNhkZidKERm419sz3Kaat4mjW6anvc4VzlsR8" width="100px;" alt="Foto do José Soares"/><br>
        <sub>
          <b>Haroldo Magano</b>
        </sub>
      </a>
    </td>
<td align="center">
      <a href="https://www.linkedin.com/in/hugo-barros-013833170/" title="UI Designer Especialista em Marketing">
        <img src="https://media.licdn.com/dms/image/C5603AQFwkCvZP4Sszg/profile-displayphoto-shrink_200_200/0/1656009934326?e=1706140800&v=beta&t=rHW0AHp9_1MNLDjn0w9vKunl1nD3I5y3rp0mGYe_CSc" width="100px;" alt="Foto do José Soares"/><br>
        <sub>
          <b>Hugo Barros</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

## 😄 Seja um dos contribuidores

Quer fazer parte desse projeto? Clique [AQUI](CONTRIBUTING.md) e leia como contribuir.

## 📝 Licença

Esse projeto está sob licença. Veja o arquivo [LICENÇA](LICENSE.md) para mais detalhes.
