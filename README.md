<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
<h3 align="center">Edgerunner</h3>

  <p align="center">
    <br />
    <a href="https://github.com/k-avi/edgerunner/issues">Report Bug</a>    ·
    <a href="https://github.com/k-avi/edgerunner/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

![product-screenshot]

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Built With

* Ncurses
* C
* A lot of coffee

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

In order to compile and play edgerunner, you'll need the ncurses library installed on your computer. 
You'll also need the gcc compiler, gnu make and a shell language.

Most distros provide a package for the library. On some distros (notably gentoo), you'll have to 
replace the 
```sh
-lcurses
```
flag by 
```sh
-lncurses -ltinfo
```
in the src/makefile file.

### Prerequisites

The only dependency that you might not have on you distro is ncurses. I don't guarantee that the code will work with a very 
old version of gcc.

* gcc
* make 
* ncurses
  
    * debian / ubuntu 
    ```sh
        sudo apt-get install libncurses-dev
    ```
    * arch 
    ```sh
        sudo pacman -S ncurses
    ```
    * void 
    ```sh
        sudo xbps-install ncurses
    ```


### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/k-avi/edgerunner.git
   cd edgerunner
   ```
2. you can use the build.sh script to compile edgerunner, add it to /usr/local/bin and add a manpage for edgerunner in /usr/local/share/man
   the script will also check for dependencies. If your linux does not have a /usr/local/share/man directory, the script won't create a man page.
   You can still copy manpages/edgerunner/man1/edgerunner.1 to your standard man location.
   ```sh
   sudo bash build.sh 
   ```
3. If you don't want to install edgerunner in /usr/local/bin , and want to create a binary simply type
   ```sh
   make edgerunner  
   ``` 
  
4. If you built edgerunner with build.sh and want to remove it , use the uninstall.sh script.
   ```sh
   sudo bash uninstall.sh 
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

![product-screenshot1]

To start a game, simply start the program. during the game, you are the 
_@_ symbol on the screen. You try to reach the _%_ symbol by moving from 
node to node on a graph. You have to be careful and avoid the ennemy (or ennemies) 
represented with the _!_ character. 
The node where the exit is is colored in green , the node where the player is is colored 
in blue, the nodes where the ennemies are are colored in red.

In order to move from your node to another, type the number appearing on the node. 
Good luck ! 

product-screenshot1

_For more informations, please refer to the manpage of the game or use the -h options_

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [ ] Items / special nodes 
- [ ] Score enhancement (better score system and score saving)
- [ ] Screensaver mode 

See the [open issues](https://github.com/k-avi/edgerunner/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the free software community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- LICENSE -->
## License

Distributed under the GPLv3 License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

I.M-R - i.m-r.ecole@orange.fr.com

Project Link: [https://github.com/k-avi/edgerunner](https://github.com/k-avi/edgerunner)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## Acknowledgments

* [Readme Template](https://github.com/othneildrew/Best-README-Template)
* [BsdRogue original paper](https://docs-archive.freebsd.org/44doc/usd/30.rogue/paper.pdf)
* [Ncurses homepage](https://invisible-island.net/ncurses)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/k-avi/edgerunner.svg?style=for-the-badge
[contributors-url]: https://github.com/k-avi/edgerunner/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/k-avi/edgerunner.svg?style=for-the-badge
[forks-url]: https://github.com/k-avi/edgerunner/network/members
[stars-shield]: https://img.shields.io/github/stars/k-avi/edgerunner.svg?style=for-the-badge
[stars-url]: https://github.com/k-avi/edgerunner/stargazers
[issues-shield]: https://img.shields.io/github/issues/k-avi/edgerunner.svg?style=for-the-badge
[issues-url]: https://github.com/k-avi/edgerunner/issues
[license-shield]: https://img.shields.io/github/license/k-avi/edgerunner.svg?style=for-the-badge
[license-url]: https://github.com/k-avi/edgerunner/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[product-screenshot]: images/screenshot_edgerunner3.png
[product-screenshot1]: images/screenshot_edgerunner1.png
