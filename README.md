# Analyzing Disk Scheduling Algorithms

## Project Overview

This repo contains a comprehensive project that focuses on comparing the efficiency and fairness of disk scheduling algorithms: FIFO, SSTF, Scan, and C-Scan. It involves generating disk track requests, implementing the algorithms, and analyzing their performance in terms of track traversal and request delay fairness.

## Features

- Random generation of disk track requests (0 to 99).
- Implementation of FIFO, SSTF, Scan, and C-Scan algorithms.
- Analysis of tracks traversed by the read/write arm.
- Evaluation of fairness in request processing order.
- Visualization of request delays and analysis via histograms.
- Calculation of longest and average delays for tracks.

## Getting Started

### Prerequisites

- Programming environment for C

### Installation

- Clone the repository to your local machine.

### Usage

1. Navigate to the project directory in your command line interface.
2. Compile the program using `make` in the shell.
3. Execute with `./main [number_of_requests]`, where `[number_of_requests]` is an integer in the range [0:99].

### Sample Output

The `input.txt` file in the directory contains the generated sequence of requests. The outputs for different request numbers are saved in `output.csv` and include the total tracks traversed and delays experienced by the requests.

## Analysis and Reports

Further analysis of the different algorithms, along with a detailed report, can be found in `analysis.md` in the repository.

## Collaboration Statement

Resources used:
- Overview of disk scheduling content: [Zybooks Chapter](https://learn.zybooks.com/zybook/ROLLINSCMS470SummetSpring2023/chapter/9/section/3)
- SSTF and C-Scan algorithm understanding: [Coding Ninjas Library](https://www.codingninjas.com/codestudio/library)
- Refreshing Merge Sort: [Programiz](https://www.programiz.com/dsa/merge-sort)
- General C syntax questions: [OpenAI Chat](https://chat.openai.com)

## Contributing

This project was developed as an extension of a class assignment, and while it is primarily for educational purposes, contributions or suggestions for improvements are welcome. To contribute:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a pull request.

## Contact
Matheus K. Westphalen - mkwestphalen@gmail.com

Project Link: [https://github.com/mwestphalen/disk-scheduling-algorithms](https://github.com/mwestphalen/disk-scheduling-algorithms)
