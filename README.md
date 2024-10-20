
# Path Finder

Dynamic path-finding demo written using SDL2. The path is restricted to left/right/up/down movements, and currently only implements the breadth-first pathfinding algorithm.

## Demo

![PathfindingDemoV3](https://github.com/user-attachments/assets/10eb8047-a26f-4d16-ac13-d9a209e5dab8)

## Installation & Running

Note: you will need SDL2 installed and on your path. Alternatively when compiling use the `-I=path/to/sdl2` parameter.

1- Clone the repo
```
  git clone https://github.com/Sam-Gledhill/PathFinder
```

```
  cd PathFinder
```


2- Run the program

Linux
```
  chmod +x run.sh && ./run.sh
```

Windows (untested)
```
  g++ main.cpp -o main.exe -lSDL2 && main.exe 
```

## Features

- Dynamic path-finding around obstacles
- Ability to draw and remove new obstacles
- Display mode for showing path-finding algorithm in real-time (WIP)
- Mulitple path-finding algorithms to choose from (WIP)
- SDL2


## Authors

- [@Sam-Gledhill](https://www.github.com/Sam-Gledhill)


## Contributing

Contributions are always welcome!

See `contributing.md` for ways to get started.


## License

[MIT](https://choosealicense.com/licenses/mit/)

