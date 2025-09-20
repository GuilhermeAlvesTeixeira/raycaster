# Raycasting Engine

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.5-green.svg)](https://www.sfml-dev.org/)
[![ImGui](https://img.shields.io/badge/ImGui-1.90%2B-orange.svg)](https://github.com/ocornut/imgui)
[![ImGui-SFML](https://img.shields.io/badge/imgui--sfml-latest-lightgrey.svg)](https://github.com/eliasdaler/imgui-sfml)

Um motor gráfico baseado em **raycasting**, inspirado em clássicos como *Wolfenstein 3D*.  
Implementado em **C++20** com **SFML**, o projeto renderiza paredes, aplica texturas, iluminação simples e névoa.  
Agora com suporte a **Dear ImGui** e **ImGui-SFML** para criar ferramentas de debug e interfaces gráficas rápidas.

## Screenshot
<img width="1279" height="713" alt="raycaster" src="https://github.com/user-attachments/assets/6ccf1668-0c28-4c1d-8290-39f9c498ad40" />

## Requisitos
- CMake >= 3.16
- C++20
- SFML >= 2.5
- [Dear ImGui](https://github.com/ocornut/imgui) v1.90.1
- [ImGui-SFML](https://github.com/eliasdaler/imgui-sfml)

## Bibliotecas utilizadas
- **SFML** → renderização, janelas e input.
- **Dear ImGui** → interface gráfica 
- **ImGui-SFML** → integração (Back-end) entre ImGui e SFML.
