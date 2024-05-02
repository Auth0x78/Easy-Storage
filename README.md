# Easy-Storage
It is a lightweight C++ class designed for simple data saving to files and loading from files. It provides efficient methods for writing data to disk and reading data from files, making it suitable for various applications where basic file-based data storage is required.

## Features
- **Efficient Data Saving:** Utilizes efficient file writing techniques to save data to disk.
- **Error Handling:** Includes error codes for common file operation failures, facilitating straightforward error handling in client code.
- **File Checksum Checking:** Calculates and verifies checksums to detect file corruption.

## Usage
1. Instantiate the `DataLoadSave` class.
2. Call the `SaveData` method to save data to a specified file.
3. Use the `LoadData` method to load data from a file into a provided buffer.

## Credits
- `xxhash64` library provided by Stephan Brumme. See http://create.stephan-brumme.com/disclaimer.html for more information.
