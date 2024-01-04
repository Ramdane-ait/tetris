# AIT MOUHOUB RAMDANE
# tetris
jeu Tetris en langage C en MVC avec des vues SDL et Ncurses

les bibliothèques **SDL** **SDL_font** et **SDL_mix** doivent être installés sur la machine pour pouvoir compiler le programme
pour compiler: exécution du makefile avec la commande make.
l'executable sera build/tetris **"sdl"**|**"text"**.

sdl : pour l'affichage en SDL.
text : pour l'affichage Ncurses. 

## fonctionnalités:
- action hold
- son en arriere plan 
- gestion des niveaux
- gestion de la gravité
- affichage de la piece suivante.
- gestion des pieces (mouvement,rotation..).

## touches: 
- flèche haut ou z: rotation dans le sens positif.
- flèche bas ou s : soft drop.
- fléche gauche ou q : mouvement à gauche
- fléche droite ou d : mouvement à droite  
- espace : hard drop
- touche c : action hold.
- ctrl droite ou a: rotation dans le sens negatif.



