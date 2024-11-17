<div align="center">
  <h3 align="center">Spline</h3>
</div>

## Sobre

Spline brrrr

## Começando

### Pré-requisitos

- Linux (sem suporte pro windows, sra. microsoft)
- [GNU Compiler Collection](https://gcc.gnu.org/)

### Instalando

1. Clone o repositório

   ```sh
   git clone ##REPLACE##
   ```

2. Dirija-se ao seu diretório

   ```sh
   cd ##REPLACE##/
   ```

3. Compile o código-fonte

   ```sh
   make
   ```

## Contribuindo

Este projeto está configurado e integrado com o Visual Studio Code.

### Valgrind

Há uma regra no [Makefile](./Makefile) para lançar rapidamente o Valgrind. O resultado será salvo em [valgrind-out.txt](./valgrind-out.txt).

```sh
make valgrind VALARGS=<args>
```

Onde,

- `args` - são os argumentos para passar ao programa. Opcional.

## Suporte

Esse programa não funciona em ambientes não-POSIX e pode até mesmo não funcionar em sistemas não-Linux.

Isso se deve ao fato de usar APIs que são extensões do POSIX e não fazem parte do padrão C.

## Licença

Licenciado sob [Unlicense](./LICENSE).
