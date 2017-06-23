# cojsTetris
Cooperative tetris game using C server and javascript webpage clients.

## About
This project relies on third-party C code for the http file server: GNU Libmicrohttpd. See https://www.gnu.org/software/libmicrohttpd/.
This projects reuses a javascript tetris game implementationnamed jsTetris, made by Mathieu CAROFF. It may (or may not) be found at the address https://github.com/mathieucaroff/jsTetris/.

## Installation
    git clone https://github.com/mathieucaroff/cojsTetris.git && cd cojsTetris
    git clone https://github.com/mathieucaroff/jsTetris.git && cd jsTetris
    git checkout cojstetris && cd ..
    sudo apt-get install libmicrohttpd-dev

If you want to have the source files locally:
    git clone https://gnunet.org/git/libmicrohttpd.git
    cd libmicrohttpd
    sudo apt-get install texinfo
    autoreconf -fi
    ./configure
    make

## Use 
In order to play this game, two persons must reach the web page http://127.0.0.1:8888. 
If you close your web page, and try to reconnect, you will have to wait 1 minute or restart the server. 

## Git (if you are a collaborator on this repository)
Install git on Ubuntu:

    sudo apt install git

Setting up git

    git config --global user.name 'your.user.name'
    git config --global user.email 'some.email@example.com'
    git config --global push.default simple

Usual command chain to upload your modifications:

    git status
    git add file1 file2
    git commit -m 'Modification description'
    git pull
    git push

Go back to specific point (commit) in log:

    git log # Displays the lasts commits
    git checkout <commit-id>

Undo the last commit:

    git reset HEAD~

Modify the last commit:

    git commit --amend

