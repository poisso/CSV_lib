#include "csvlib.h"

Csv::Csv(std::string path, std::vector<std::vector<std::string>> content, char sep){
    std::ofstream file (path);
    if (!content.empty()){
        for (uint32_t i = 0; i < content.size(); i++){
            for (uint32_t j = 0; j < content[0].size() - 1; j++){
                file << content[i][j] << sep;
        }
        //we add the last term out of the loop to avoid having ";" at the end of the line
        file << content[i][content[0].size() - 1];
        file << std::endl;
        }
        this->n_columns = content[0].size();
        this->n_lines = content.size();
    }
    else{
        this->n_columns = 0;
        this->n_lines = 0;
    }

    this->path = path;
    this->sep = sep;
    this->content = content;
}

Csv::Csv(std::string path, char sep){
    //we create a stream with the file at location path
    std::ifstream file(path);

    //these 3 variable will be used during the parsing of the file
    //elems will store the separated content of the line
    //item will store the separate values to be stored in elems
    //line will store the initial content of each line as a string
    std::vector<std::string> elems;
    std::string line;
    std::string item;

    //content_file will store the result of the parsing of the file
    std::vector<std::vector<std::string>> content_file;

    uint32_t line_counter = 0;
    uint32_t column_counter = 0;
    //we go through the line of the file
    while (std::getline(file, line)){
        std::stringstream ss(line);
        //we go through the columns of the file
        while (std::getline(ss, item, sep)) {
            elems.push_back(item);

        }
        //to treat the case where the last element of the line is a semi colon
        //we have to add an extra empty element to the vector containing the line to get the right count
        if (line[line.size() - 1] == sep){
            elems.push_back("");
        }
        try{
            if ((column_counter != 0) && ( column_counter != elems.size()))
            {
                throw std::string("The lines of this csv file are not all the same size");
            }
        }

        catch(std::string const& chaine)
        {
            std::cerr << chaine << std::endl;
        }
        column_counter = elems.size();
        content_file.push_back(elems);
        elems.clear();
        line_counter++;
    }

    //we init the different attributes of the object Csv
    this->n_columns = column_counter;
    this->n_lines = line_counter;
    this->content = content_file;
    this->path = path;
    this->sep = sep;

}

std::vector<uint32_t> Csv::dimension_get(){
    std::vector<uint32_t> result;
    result.push_back(this->n_columns);
    result.push_back(this->n_lines);
    return result;
}

char Csv::sep_get(){
    return this->sep;
}

std::vector<std::vector<std::string>> Csv::content_get(){
    return this->content;
}

std::ostream& operator<<(std::ostream& os, Csv file){
for (uint32_t i = 0; i < file.content.size(); i++){
    for (uint32_t j = 0; j < file.content[0].size(); j++){
        os << file.content[i][j] << " ";
    }
    os << std::endl;
}
return os;
}

void Csv::file_remover(){
    int status;
    status = remove(this->path.c_str());
    try{
        if (status){
            throw std::string("The file could not be removed");
        }
    }
    catch(std::string const& chaine){
        std::cout << chaine << std::endl;
    }
}

std::vector<std::string> Csv::column_extractor(std::string column_name){
    std::vector<std::string> column;
    int64_t column_index = -1;
    for (uint32_t i = 0; i < this->content[0].size(); i++){
        if (this->content[0][i] == column_name){
            column_index = i;
            break;
        }
    }
    if (column_index != -1){
        //we start at 0 so the output contains the column name
        for (uint32_t k = 0; k < this->content.size(); k++){
            column.push_back(this->content[k][column_index]);
        }
        return column;
    }
    else{
        std::string error("ERROR:no matching columns");
        std::vector<std::string> er;
        er.push_back(error);
        return er;
    }

}

std::vector<std::string> Csv::line_extractor(std::string line_name){
    std::vector<std::string> line;
    int64_t line_index = -1;
    for (uint32_t i = 0; i < this->content.size(); i++){
        if (this->content[i][0] == line_name){
            line_index = i;
            break;
        }
    }
    if (line_index != -1){
        //we start at 0 so the output contains the column name
        for (uint32_t k = 0; k < this->content[0].size(); k++){
            line.push_back(this->content[line_index][k]);
        }
        return line;
    }
    else{
        std::string error("ERROR:no matching columns");
        std::vector<std::string> er;
        er.push_back(error);
        return er;
    }
}

void Csv::erase_column(std::string colname){
    assert(this->n_columns >= 1);
    assert(this->n_lines >= 1);
    std::vector<std::string> column;
    int64_t column_index = -1;
    for (uint32_t i = 0; i < this->content[0].size(); i++){
        //if the column name is not in the table nothing happens
        if (this->content[0][i] == colname){
            column_index = i;
            break;
        }
    }
    if (column_index != -1){
        for (uint32_t k = 0; k < this->content.size(); k++){
            this->content[k].erase(this->content[k].begin() + column_index);
        }
        this->n_columns--;
    }

}

void Csv::add_column(std::vector<std::string> column){
    assert(this->n_lines == column.size());
    for (uint32_t i = 0; i < this->n_lines; i++){
        this->content[i].push_back(column[i]);
    }
    this->n_columns++;
}

void Csv::line_inc(){
    this->n_lines++;
}

void Csv::column_inc(){
    this->n_columns++;
}

void Csv::line_dec(){
    this->n_lines--;
}

void Csv::column_dec(){
    this->n_columns--;
}

void Csv::operator+(Csv file){
    assert(this->n_lines == file.dimension_get()[1]);
    for (auto colname:file.content_get()[0]){
        if (std::find(this->content[0].begin(), this->content[0].end(), colname) != this->content[0].end()){
            file.erase_column(colname);
            file.column_dec();
        }
    }
    for(uint32_t i = 0; i < file.dimension_get()[0]; i++){
        std::vector<std::string> column = file.column_extractor(file.content_get()[0][i]);
        this->add_column(column);
    }
}

void Csv::apply_changes(){
    std::ofstream file(this->path, std::ofstream::trunc);
    for (uint32_t i = 0; i < this->content.size(); i++){
        for (uint32_t j = 0; j < this->content[0].size() - 1; j++){
            file << this->content[i][j] << ";";
        }
        file << this->content[i][this->content[0].size() - 1] << std::endl;
    }
}

void Csv::line_content_init(Csv file){
    if (this->content.empty()){
        std::vector<std::string> empt;
        for (uint32_t i = 0; i < file.dimension_get()[1]; i++){
            this->content.push_back(empt);
        }
        this->n_lines = file.dimension_get()[1];
    }
}


void Csv::transpose(){
    std::vector<std::vector<std::string>> new_content(this->n_columns, std::vector<std::string>(this->n_lines, "0"));
    for (uint64_t i = 0; i < this->n_lines; i++){
        for (uint64_t j = 0; j < this->n_columns; j++){
            new_content[j][i] = this->content[i][j];
        }
    }
    uint64_t temp = this->n_columns;
    this->n_columns = this->n_lines;
    this->n_lines = temp;
    this->content = new_content;
}
