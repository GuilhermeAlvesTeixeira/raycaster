# Hellcast - Raycasting Engine

Um motor gráfico baseado em **raycasting**, inspirado em clássicos como *Wolfenstein 3D*.  
Implementado em **C++20** com **SFML**, o projeto renderiza paredes, aplica texturas, iluminação simples e névoa.

## Screenshot
<img width="1193" height="670" alt="hellcast" src="https://github.com/user-attachments/assets/d9c99284-4763-4300-b102-f5e8a42cd384" />


## Requisitos
- CMake >= 3.16
- C++20
- SFML >= 2.5

## Como compilar
```bash
git clone https://github.com/seu-usuario/hellcast.git
cd hellcast
cmake -B build
cmake --build build
./build/raycaster