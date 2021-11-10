#ifndef CSVLIB_H_INCLUDED
#define CSVLIB_H_INCLUDED

#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ostream>
#include <cassert>
#include <algorithm>

class Csv{
private:
    std::string path;
    std::vector<std::vector<std::string>> content;
    char sep;
    uint32_t n_columns;
    uint32_t n_lines;
public:
    /** @brief Constructor for the object Csv (built from a vector of vector)
     *
     * @param the path to the file in which the data should be writen
     * @param the data to be written in the file
     * @param the separator to separate the data
     *
     */
    Csv(std::string path, std::vector<std::vector<std::string>> content, char sep);
    /** @brief Constructor for the object Csv (built from a file)
     *
     * @param path to the file from which the data should be extracted
     * @param the separator to separate the data
     *
     */
    Csv(std::string path, char sep);

    /** @brief getter for the dimensions of the data
     *
     * @return number of columns and number of lines of the data
     *
     */
    std::vector<uint32_t> dimension_get();
    /** @brief getter for the content of the data
     *
     * @return a vector of vector of strings containing the data
     *
     */
    std::vector<std::vector<std::string>> content_get();
    /** @brief getter for the separator
     * @return the separator
     *
     */
    char sep_get();
    /** @brief increments the number of lines by 1
     *
     */

    void line_inc();
    /** @brief increments the number of columns by 1
     *
     */
    void column_inc();
    /** @brief decrements the number of lines by 1
     *
     */
    void line_dec();
    /** @brief decrements the number of column by 1
     *
     */
    void column_dec();

    /** @brief removes the file associated to the object
     *
     */
    void file_remover();

    /** @brief extracts a column with a given name
     *
     * @param column name to extract
     * @return a vector containing the column
     *
     */
    std::vector<std::string> column_extractor(std::string column_name);

    /** @brief extract a line with a given name
     *
     * @param name of line to extract
     * @return a vector containing the line
     *
     */

    std::vector<std::string> line_extractor(std::string line_name);

    /** @brief erase a column with a given name from the data
     *
     * @param column name
     *
     */
    void erase_column(std::string colname);
    /** @brief adds a column as the las column of the data
     *
     * @param vector containing the column to be added
     *
     */

    void add_column(std::vector<std::string> column);

    /** @brief overloading of the + operator to be able to merge data
     *
     * @param Csv file to be merged with the current one
     *
     */

    void operator+(Csv file);

    /** @brief apply changes made to the content matrice to the file
     *
     */

    void apply_changes();

    /** @brief transforms the original object into it's transpose
     *
     */

    void transpose();

    /** @brief only useful for very specific cases. If the csv object is empty (empty content)
     * it initialises it's number of lines with the number of lines of file and with empty lines.
     *
     * @param Csv file from which we take the number of lines
     *
     */

    void line_content_init(Csv file);

    /** @brief overloading the << operator for diplaying purposes
     *
     * @param stream to which the data should be sent
     * @param Csv object to extract the data from
     * @return stream for diplay (mainly)
     *
     */

    friend std::ostream& operator<<(std::ostream& os, Csv file);

};

#endif // CSVLIB_H_INCLUDED
