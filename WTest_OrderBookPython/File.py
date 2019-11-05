from .OrderItem import *

class orders_file:
    """Simple wrapper for a file of orders."""
    def __init__(self):
        self._f = None # Stream to read.

    def __del__(self):
        """Dtor."""
        self.close()

    def close(self):
        """Closes file if opened."""
        if self.is_opened():
            self._f.close()
            self._f = None

    def open_for_read(self, file_name): # File name to open.
        """Opens file for read."""
        self.close()
        self._f = open(file_name, 'r')
        if not self.is_opened():
            raise Exception("Cannot open file.")

    #/// <param name="record"> Reference to a record to write data in. </param>
    #/// <returns> Returns false if end of file reached, true otherwise. </returns>
    def read_order_record(self, order_record record):
        """Reads and validates orders records."""
        if not self.is_opened():
            raise Exception('File is not opened.')

        return _stream.eof() ? false : !!(_stream >> record)

    #/// <summary> Checks if file is opened. </summary>
    #/// <returns> Returns true if the file is in opened state. </returns>
    def is_opened(self):
        return self._f