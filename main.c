#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Input generator that produces a file containing m random integers in the
 * range [0:99]
 *
 * @param m - number of tracks to be generated
 * @return the track where the disk will start rotating from
 */
int generateInput(int m) {
  // Declare pointer of type FILE
  FILE *fptr;

  // Creating file for writing
  fptr = fopen("input.txt", "w");

  // Check for file creation error
  if (fptr == NULL) {
    printf("Error creating the file!");
    exit(1);
  }

  // Write # of requests to file
  fprintf(fptr, "%d\n", m);

  // Seeding rand() with # of ticks since program was launched (better
  // randomness)
  srand((unsigned)clock());

  // Create m random integers in the range [0:99]
  for (int i = 0; i < m; i++) {
    // Generate number
    int num = rand() % 100;

    // Write number to file
    fprintf(fptr, "%d\n", num);
  }

  // Close file
  fclose(fptr);

  // Supply random disk start location (within the given boundaries)
  int startLocation = rand() % 100;

  return startLocation;
}

/* Gets the values from the input.txt file
 *
 * @param filename - the name of the file
 * @return an array with all the values read from the file
 */
int *readRequestsFromFile(const char *filename) {
  // Declare pointer of type FILE
  FILE *fptrInput;

  // Open file for reading
  fptrInput = fopen(filename, "r");

  // Check for error when opening file
  if (fptrInput == NULL) {
    printf("Error creating the file!");
    exit(1);
  }

  // Get number of requests to be served (1st line in .txt file)
  int numOfRequests;
  if (fscanf(fptrInput, "%d", &numOfRequests) == 0) {
    printf("Error reading file.1\n");
  }

  // Place all tracks in array (including its size as the first element)
  int *values = malloc(sizeof(int) + (numOfRequests * sizeof(int)));
  values[0] = numOfRequests;

  int trackNum;
  for (int i = 1; i < numOfRequests + 1; i++) {
    if (fscanf(fptrInput, "%d", &trackNum) == 0) {
      printf("Error reading file.\n");
    }

    values[i] = trackNum;
  }

  // Tracks hold the # of requests as its first element, and the other values
  // subsequently
  return values;
}
/* Indicates whether there are any more tracks to be serviced
 * @param arr[] - array of tracks
 * @param size - size of array
 * @return 1 if there are still tracks to be serviced, 0 if not.
 */
int hasPendingRequests(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    if (arr[i] != -1) {
      return 1;
    }
  }
  return 0;
}

/////////////////////////////////////////////////////
// Sorting Algorithm to help in DSA implementation //
/////////////////////////////////////////////////////

/* Merges the left side of the array with the right side
 * @param arr[] - array of tracks
 * @param left - leftmost index of the array
 * @param midpoint - the midpoint of the array
 * @param right - rightmost index of the array
 */
void merge(int arr[], int left, int midpoint, int right) {
  // Get size of first half (left)
  int sizeLeft = midpoint - left + 1;
  // Get size of second half (right)
  int sizeRight = right - midpoint;

  // Declare two temporary arrays
  int leftArr[sizeLeft];
  int rightArr[sizeRight];

  // Copy data from original array to temporary arrays
  for (int i = 0; i < sizeLeft; i++) {
    leftArr[i] = arr[left + i];
  }

  for (int j = 0; j < sizeRight; j++) {
    rightArr[j] = arr[midpoint + 1 + j];
  }

  // Keep current index of sub-arrays and main array
  int i = 0;
  int j = 0;
  int k = left;

  // Until we reach the end of either sub-array, pick the bigger
  // values amongst both
  while (i < sizeLeft && j < sizeRight) {
    if (leftArr[i] <= rightArr[j]) {
      arr[k] = leftArr[i];
      i++;
    } else {
      arr[k] = rightArr[j];
      j++;
    }
    k++;
  }

  // Copy remaining items from leftArray into arr
  while (i < sizeLeft) {
    arr[k] = leftArr[i];
    i++;
    k++;
  }

  // Copy remaining items from rightArray into arr
  while (j < sizeRight) {
    arr[k] = rightArr[j];
    j++;
    k++;
  }
}

/* Merge sort algorithm, using a Divide and Conquer technique
 * @param arr[] - array of tracks
 * @param left - the leftmost index of the array
 * @param right - rightmost index of the array
 */
void mergeSort(int arr[], int left, int right) {
  if (left < right) {
    // Find the mid value in the array
    int midpoint = (right + left) / 2;

    // Call the merge sort for the first half
    mergeSort(arr, left, midpoint);

    // Call merge sort for the second half
    mergeSort(arr, midpoint + 1, right);

    // Merge both halves
    merge(arr, left, midpoint, right);
  }
}

///////////////////////////////////////////////
// Disk Scheduling Algorithms implementation //
///////////////////////////////////////////////

/* First In First Out (FIFO)
 * @return - total number of tracks traversed
 */
int tracksTraversedByFIFO(int startLocation) {
  /*
   *note*: Another way to do the following is to first read all the numbers into
   *an array, and then work with it to calculate the total tracks traversed.
   */
  
  // Keep track of total tracks traversed
  int total = 0;

  // Declare pointer of type FILE
  FILE *fptr;

  // Open file for reading
  fptr = fopen("input.txt", "r");

  // Check for error when opening file
  if (fptr == NULL) {
    printf("Error creating the file!");
    exit(1);
  }

  // Get number of requests to be served (1st # in .txt file)
  int size;
  if (fscanf(fptr, "%d", &size) == 0) {
    printf("Error reading file.\n");
  }

  // Read in first track #
  int currentTrack;
  if (fscanf(fptr, "%d", &currentTrack) == 0) {
    printf("Error reading file\n");
  }

  // Starting the disk at a random track, traverse to the first track
  total += abs(startLocation - currentTrack);

  for (int i = 0; i < size; i++) {
    // Read in the next track #
    int nextTrack;
    if (fscanf(fptr, "%d", &nextTrack) == 0) {
      printf("Error reading file\n");
    }

    // Find how many track will be traversed from the current -> next
    total += abs(nextTrack - currentTrack);

    // Go to the next track
    currentTrack = nextTrack;
  }

  // Total number of tracks traversed
  return total;
}

/* Shortest Seek Time First (SSTF)
 * @param startLocation where the disk should start to service its requests
 * @return - array with first value being # of tracks traversed and subsequent
 values being delay values experienced by any request as a result of being
 processed out of FIFO order
 */
int *tracksTraversedBySSTF(int startLocation) {
  // Keep track of total tracks traversed
  int total = 0;

  // Read input from .txt file (all values w/ first one being the # of requests)
  int *tracks = readRequestsFromFile("input.txt");
  int numOfRequests = tracks[0];
  tracks++;

  // Used to preserve original order requests came through
  int originalOrder[numOfRequests];

  // Place all requests in array
  for (int i = 0; i < numOfRequests; i++) {
    originalOrder[i] = tracks[i];
  }

  // Array to track the order the requests were served
  int *orderServiced = malloc(numOfRequests * sizeof(int));
  int iter = 0;

  // Set the current track as the track the disk started in
  int currentTrack = startLocation;

  // Sort requests in ascending order
  mergeSort(tracks, 0, numOfRequests - 1);

  // Find where the current track is and service it
  for (int i = 0; i < numOfRequests; i++) {
    if (tracks[i] == currentTrack) {
      tracks[i] = -1;
    }
  }

  while (hasPendingRequests(tracks, numOfRequests)) {
    int shortestSeekTime = 999;
    int nextTrack = -1;
    int SSTindex = 0;
    int seekTime = 0;
    // Find the next track to be serviced (the one with the SST)
    for (int i = 0; i < numOfRequests; i++) {
      if (tracks[i] != -1) {
        seekTime = abs(currentTrack - tracks[i]);

        if (seekTime < shortestSeekTime) {
          shortestSeekTime = seekTime;
          nextTrack = tracks[i];
          SSTindex = i;
        }
      }
    }

    // If no more track were found, we've reached the end
    if (nextTrack == -1) {
      break;
    }

    // Update number of tracks traversed
    total += abs(currentTrack - nextTrack);

    // Move the disk arm to the next track
    currentTrack = nextTrack;

    // Copy track serviced into array of serviced tracks
    orderServiced[iter] = tracks[SSTindex];
    iter++;

    // Mark the new current track as serviced (-1)
    tracks[SSTindex] = -1;
  }

  // Initialize array of delay values
  int delayValues[numOfRequests];
  for (int i = 0; i < numOfRequests; i++) {
    delayValues[i] = 0;
  }
  // Calculate any delay experienced by comparing the order requests came in
  // with the order they were attended
  for (int i = 0; i < numOfRequests; i++) {
    for (int j = 0; j < numOfRequests; j++) {
      if (originalOrder[i] == orderServiced[j]) {
        // Only concerned about the ones that were delayed, not sped up
        if (j > i) {
          delayValues[i] = j - i;
        }
      }
    }
  }

  // Pack # of tracks traversed and delays values into array
  int *values = malloc(sizeof(int) + (numOfRequests * sizeof(int)));
  values[0] = total;
  for (int i = 1; i < numOfRequests + 1; i++) {
    values[i] = delayValues[i - 1];
  }
  // Return list of values
  return values;
}

/* C-Scan Scheduling Algorithm
 * @param startLocation where the disk should start to service its requests
 * @return - array with first value being # of tracks traversed and subsequent
 * values being delay values experienced by any request as a result of being
 * processed out of FIFO order
 */
int *trackTraversedByCScan(int startLocation) {
  // Keep track of total tracks traversed
  int total = 0;

  // Read input from .txt file (all values w/ first one being the # of requests)
  int *tracks = readRequestsFromFile("input.txt");
  int numOfRequests = tracks[0];
  tracks++;

  // Used to preserve original order requests came through
  int originalOrder[numOfRequests];

  // Place all requests in array
  for (int i = 0; i < numOfRequests; i++) {
    originalOrder[i] = tracks[i];
  }

  // Array to track the order the requests were served
  int *orderServiced = malloc(numOfRequests * sizeof(int));
  int iter = 0;

  // Set the current track as the track the disk started in
  int currentTrack = startLocation;

  // Sort tracks in ascending order
  mergeSort(tracks, 0, numOfRequests - 1);

  while (hasPendingRequests(tracks, numOfRequests)) {
    int nextTrack = -1;

    // Find the next track to be serviced
    for (int i = 0; i < numOfRequests; i++) {
      if (tracks[i] >= currentTrack) {
        nextTrack = tracks[i];
        break;
      }
    }
    // If no more tracks were found in the current direction, go back to the
    // to the opposite end
    if (nextTrack == -1) {
      nextTrack = tracks[0];
    }

    // If the next track is the track we started in, all tracks have beens
    // serviced
    if (nextTrack == -1) {
      break;
    }

    // Update number of tracks traversed
    total += abs(nextTrack - currentTrack);

    // Move the disk arm to the next track
    currentTrack = nextTrack;

    // Copy track serviced into array of serviced tracks
    orderServiced[iter] = nextTrack;
    iter++;

    // Mark the current track as serviced
    for (int i = 0; i < numOfRequests; i++) {
      if (tracks[i] == currentTrack) {
        tracks[i] = -1;
      }
    }
  }

  // Initialize array of delay values
  int delayValues[numOfRequests];
  for (int i = 0; i < numOfRequests; i++) {
    delayValues[i] = 0;
  }

  // Calculate any delay experienced by comparing the order requests came in
  // with the order they were attended
  for (int i = 0; i < numOfRequests; i++) {
    for (int j = 0; j < numOfRequests; j++) {
      if (originalOrder[i] == orderServiced[j]) {
        // Only concerned about the ones that were delayed, not sped up
        if (j > i) {
          delayValues[i] = j - i;
        }
      }
    }
  }

  // Pack # of tracks traversed and delays values into array
  int *values = malloc(sizeof(int) + (numOfRequests * sizeof(int)));
  values[0] = total;
  for (int i = 1; i < numOfRequests + 1; i++) {
    values[i] = delayValues[i - 1];
  }

  // Return list of values
  return values;
}

int main(int argc, char *argv[]) {
  // Display prompt
  printf("\n***Disk Scheduling Algorithms Project***\n\n");

  // Check if there are any cmd line arguments
  if (argc != 2) {
    printf("!Error!\nMust specify one argument: Number of requests to be "
           "serviced [0:99].\n\n");
    exit(0);
  }

  // Read number of requests from cmd line
  int numOfRequests = atoi(argv[1]);

  // Check if argument is within the boundaries
  if (numOfRequests < 0 || numOfRequests > 99) {
    printf("!Error!\nNumber of requests to be "
           "serviced must be within the range [0:99].\n\n");
    exit(0);
  }

  // Generated input can be located in input.txt
  int startLocation = generateInput(numOfRequests);

  /*
   * Run disk scheduling algorithms
   */
  int tracksTraversed_FIFO = tracksTraversedByFIFO(startLocation);
  int *resultsSSTF = tracksTraversedBySSTF(startLocation);
  int *resultsCScan = trackTraversedByCScan(startLocation);

  // Get # of tracks traversed by SSTF and C-Scan
  int tracksTraversed_SSTF = resultsSSTF[0];
  int tracksTraversed_CScan = resultsCScan[0];

  /*
   * Create and set up .csv file for delay values output
   */

  // Declare pointer of type FILE
  FILE *fptr;

  // Creating file for writing
  fptr = fopen("output.csv", "w");

  // Check for file creation error
  if (fptr == NULL) {
    printf("Error creating the file!");
    exit(1);
  }

  // Labeling the .csv file the results
  int *requests = readRequestsFromFile("input.txt");

  // Writing column headers
  fprintf(fptr, "Track #,FIFO Delay,SSTF Delay,C-SCAN Delay\n");

  // Writing requests + delay values experienced by each algorithm
  int size = requests[0];
  for (int i = 1; i < size + 1; i++) {
    fprintf(fptr, "%d,", requests[i]);
    fprintf(fptr, "%d,", 0); // FIFO does not experience delays (delays = 0)
    fprintf(fptr, "%d,", resultsSSTF[i]);
    fprintf(fptr, "%d,\n", resultsCScan[i]);
  }

  // Close file
  fclose(fptr);

  // Comparison on the # of tracks traversed by each algorithm
  printf("\nThe following displays the number of tracks traversed by each "
         "algorithm"
         ".\nPlease check input.txt for the inputs used, and output.csv for "
         "more details.\n");

  printf("\nDisk started at track: %d\n", startLocation);
  printf("Tracks traversed by the disk arm using FIFO: %d\n",
         tracksTraversed_FIFO);
  printf("Tracks traversed by the disk arm using SSTF: %d\n",
         tracksTraversed_SSTF);
  printf("Tracks traversed by the disk arm using C-Scan: %d\n",
         tracksTraversed_CScan);

  return 0;
}
