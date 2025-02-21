//212-Chernyshova-Svetlana- Gauss - main

#include "group_212_Chernyshova_Svetlana_Gauss_functions.h"
using namespace Gauss_program;
//212-Chernyshova-Svetlana

    //class Init_field
// Constructor --

    int Init_field::initialize()
    //212-Chernyshova-Svetlana
    {
        int j;
        int i; // to compare with size_t
        //delete field_arr;
        //printf("5\n");
        //delete field_arr;
        field_arr = new int[int(x_size * y_size)];
        //printf("3\n");
        if (field_arr == nullptr) { return FAIL_INIT; }
        for (i = 0; i < int(x_size * y_size); i++)
        {
            field_arr[i] = 127; // всего 256
        }
        
        weight_matrix.resize(int(x_size), std::vector<double>(int(y_size), 0));
        for (i = 0; i < int(x_size); i++)
        {
            for (j = 0; j < int(y_size); j++)
            {
                weight_matrix[i][j] = 0;
            }
        }

        initialization_worked = 1;
        return SUCCESS_INIT;
    }

    int Init_field::initialize_with_zeros()
    {
        int i,j; // to compare with size_t
        field_arr = new int[int(x_size * y_size)];
        ///printf("3\n");
        if (field_arr == nullptr) { return FAIL_INIT; }
        for (i = 0; i < x_size * y_size; i++)
        {
            field_arr[i] = 0; // всего 256
        }

        weight_matrix.resize(int(x_size), std::vector<double>(int(y_size), 0));
        for (i = 0; i < int(x_size); i++)
        {
            for (j = 0; j < int(y_size); j++)
            {
                weight_matrix[i][j] = 0;
            }
        }

        initialization_worked = 1;
        return SUCCESS_INIT;
    }


// получить значение в клеточке - возможно лишняя функция
    size_t Init_field::get_x_size() const {return x_size;}
    size_t Init_field::get_y_size() const {return y_size;}
    void Init_field::set_x_size(size_t n_x) { x_size = n_x; }
    void Init_field::set_y_size(size_t n_y) { y_size = n_y; }
    int Init_field::get_field_value(int x, int y)
    {
        return field_arr[x * y_size + y]; //надеюсь верный индекс
    }
// изменить значение в клеточке на КОНКРЕТНОЕ, подаваемое как параметр
    void Init_field::set_definite_field_value(int x, int y, int set_definite_value)
    //212-Chernyshova-Svetlana
    //sets the value "set_definite_value" unless it is out of range
    {
        if ((0 <= set_definite_value) && (set_definite_value <= 255))
        {
            field_arr[x * y_size + y] = set_definite_value;
        }
        else if (set_definite_value < 0)
        {
            field_arr[x * y_size + y] = 0;
        }
        else if (set_definite_value > 255)
        {
            field_arr[x * y_size + y] = 255;
        }
    }
// изменить значение в клеточке при добавлении Гаусса
    void Init_field::set_field_value(int x, int y, int set_value)
    //212-Chernyshova-Svetlana
    //sets the value using normal distribution
    {
        //int w = (double((set_value - 127) + (field_arr[x * int(y_size) + y] - 127)))/2.;
        //int l = w + 127;
        int l = set_value;
        //set_value = set_value - 127;
        //int value = field_arr[x * int(y_size) + y] - 127;
        int value = field_arr[x * int(y_size) + y];
        //if (value > 10 || value < -10)
        
        //if (value != 0)
        //{
            l = (set_value + value * weight_matrix[x][y])/(weight_matrix[x][y] + 1);
            weight_matrix[x][y] += 1;
            //l = l + 127;
        //}
        // else ничего, тк l = set_value;
        
        field_arr[x * int(y_size) + y] = l; // иначе будет странно выглядеть, когда несколько гауссов друг на друга накладываются
        ///printf("field_arr : %d\n", field_arr[x * y_size + y]);
    }

    int Init_field::gnuplot(char * GNU_data_file_name, char * GNU_file_name) const
    //212-Chernyshova-Svetlana
    //makes gnuplot files
    {
        FILE * GNU_data_file, * GNU_file;
        //int si_x, si_y;
        int value;
        int i, j;
        
        GNU_data_file = fopen(GNU_data_file_name, "w");
        GNU_file = fopen(GNU_file_name, "w");
        for (i = 0; i < int(x_size); i++)
        {
            for (j = 0; j < int(y_size); j++)
            {
                value = field_arr[i * y_size + j];
                fprintf(GNU_data_file, "%d %d %d\n", i, j, value);
            }
            fprintf(GNU_data_file, "\n");
        }
        fprintf(GNU_file, "set terminal png size 2000,2000\n\
set output 'plot.png'\n\
set pm3d\n\
set palette rgbformulae 7,5,15\n\
set xlabel \"X\"\n\
set ylabel \"Y\"\n\
set zlabel \"Z\"\n\
set xrange [0:%d]\n\
set yrange [0:%d]\n\
set zrange [0:255]\n\
splot \"%s\" u 1:2:3 with dots", int(x_size), int(y_size), GNU_data_file_name);
        fclose(GNU_data_file);
        fclose(GNU_file);
        return SUCCESS_GNU;
    }


//!!!
//212-Chernyshova-Svetlana
//makes bmp

    #pragma pack(push, 1) //alignment (выравнивание) of data structure in memory
    //push saves the current alignment state to the stack

    struct BMPInfoHeader 
    {
        uint32_t biSize = sizeof(BMPInfoHeader); //razmer samoy structuri
        int32_t biWidth; //shirina kartinki v pixelyah
        int32_t biHeight; //visota kartinki v pixelyah, otricatelnoe znachenie chtobi bilo pravilnoe otobrazhenie
        uint16_t biPlanes = 1; //kolichestvo ploskostey
        uint16_t biBitCount = 24; //kolichestvo bit na 1 pixel (RGB)
        uint32_t biCompression = 0; //tip sjatiya
        uint32_t biSizeImage = 0; //razmer kartinki v baitah
        int32_t biXPelsPerMeter = 0; //gorizontalnoe razreshenie
        int32_t biYPelsPerMeter = 0; //vertikalnoe razreshenie
        uint32_t biClrUsed = 0; //kolichestvo ispolzuemih cvetov
        uint32_t biClrImportant = 0; //kolichestvo vazhnix cvetov
    }; //struct for information about the image

    struct BMPHeader 
    {
        uint16_t bfType = 0x4D42; // 'BM' , opredelyaet tip faila, 0x4D42 v 16-richnom predstavlenii
        uint32_t bfSize; //razmer faila v baitah
        uint16_t bfReserved1 = 0; //zarezervirovan i = 0
        uint16_t bfReserved2 = 0; //zarezervirovan i = 0
        uint32_t bfOffBits = sizeof(BMPHeader) + sizeof(BMPInfoHeader); //eto pole pokazivaet gde nachinaetsya bitoviy massiv otnositelno nachala faila, kotoriy i opisivaet kartinku
    }; //struct for zagolovok BMP

    #pragma pack(pop)
    //restores previous state


    int Init_field::make_bmp(const char * BMP_file_name)
    {
        int x, y;
        for (x = 0; x < int(x_size); x++)
        {
            for (y = 0; y < int(y_size); y++)
            {
                if (field_arr[x * y_size + y] > 255)
                {
                    field_arr[x * y_size + y] = 255;
                }
                else if (field_arr[x * y_size + y] < 0)
                {
                    field_arr[x * y_size + y] = 0;
                }
            }
        }

        BMPHeader header;
        BMPInfoHeader infoHeader;
        //sozdanie obektov zagolovka i informacii ob izobrazhenii BMP

        header.bfSize = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + x_size * y_size * 3;
        infoHeader.biWidth = x_size;
        infoHeader.biHeight = - y_size; // otrizatelnoe znachenie dlya pravilnogo otobrazheniya

        //zapolnenie poley zagolovka i informacii o bmp
        //bfSize zapolnyaetsya s uchetom zagolovkov i dannih izobrazheniya

        std::ofstream BMP_file(BMP_file_name, std::ios::binary);
//otkritie faila dlya zapisi v binarnom rezhime

        if (!BMP_file) 
        {
            return FAIL_BMP_opening;
        } //proverka na fail
//preobrazovivaem odin tip dannyh v drugoy s pomoshyu reinterpret_cast
        BMP_file.write(reinterpret_cast<const char*>(&header), sizeof(header)); // можно если что пробовать static_cast
        BMP_file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader)); // можно если что пробовать static_cast
//zapis zagolovka i informacii v BMP

        for (int y = 0; y < int(y_size); y++)
        {
            for (int x = 0; x < int(x_size); x++)
            {
//ispolzovanie std max i std min garantiruet, chto mi ne viydem za predelu diapazona RGB

                uint8_t r = static_cast<uint8_t>(field_arr[y * y_size + x]); //f[y][x]
                uint8_t g = static_cast<uint8_t>(field_arr[y * y_size + x]);
                uint8_t b = static_cast<uint8_t>(field_arr[y * y_size + x]);

                BMP_file.write(reinterpret_cast<const char*>(&b), sizeof(uint8_t)); //snachala siniy
                BMP_file.write(reinterpret_cast<const char*>(&g), sizeof(uint8_t)); //potom zeleniy
                BMP_file.write(reinterpret_cast<const char*>(&r), sizeof(uint8_t)); //potom krasniy
            }
            for (int i = 0; i < (4 - (int(x_size) * 3) % 4) % 4; ++i)
            {
                uint8_t padding = 0;
                BMP_file.write(reinterpret_cast<const char*>(&padding), sizeof(uint8_t));
            }
        }
        BMP_file.close();
        return SUCCESS_BMP_WRITE;
    }


    int Init_field::make_bmp_clusters(const char * filename, int k){ // Запись BMP
        const int BMP_HEADER_SIZE = 54;
        const int PIXEL_SIZE = 3;
        int file_size = BMP_HEADER_SIZE + PIXEL_SIZE * x_size * y_size;

        unsigned char bmp_header[BMP_HEADER_SIZE] = { 0 };

        // Заголовок BMP файла
        bmp_header[0] = 'B';
        bmp_header[1] = 'M';
        bmp_header[2] = file_size & 0xFF;
        bmp_header[3] = (file_size >> 8) & 0xFF;
        bmp_header[4] = (file_size >> 16) & 0xFF;
        bmp_header[5] = (file_size >> 24) & 0xFF;
        bmp_header[10] = BMP_HEADER_SIZE;

        // Заголовок DIB
        bmp_header[14] = 40; // Размер заголовка DIB
        bmp_header[18] = y_size & 0xFF;
        bmp_header[19] = (y_size >> 8) & 0xFF;
        bmp_header[20] = (y_size >> 16) & 0xFF;
        bmp_header[21] = (y_size >> 24) & 0xFF;
        bmp_header[22] = x_size & 0xFF;
        bmp_header[23] = (x_size >> 8) & 0xFF;
        bmp_header[24] = (x_size >> 16) & 0xFF;
        bmp_header[25] = (x_size >> 24) & 0xFF;
        bmp_header[26] = 1; // Число цветовых плоскостей
        bmp_header[28] = 24; // Количество бит на пиксель

        std::ofstream bmp_file(filename, std::ios::binary);
        bmp_file.write(reinterpret_cast<char*>(bmp_header), BMP_HEADER_SIZE);

        // Записываем пиксели (матрицу)
        if (k == 1)
        { 
            //Для ч\б картинки
            for (int i = x_size - 1; i >= 0; --i) {
                for (int j = 0; j < int(y_size); ++j) {
                    int value = static_cast<int>(field_arr[i*y_size + j] * 100); // Умножаю на коэффициент 100, чтобы отображалось красиво и ярко
                    unsigned char pixel[3] = { static_cast<unsigned char>(std::min(std::max(value, 0), 255)),
                                            static_cast<unsigned char>(std::min(std::max(value, 0), 255)),
                                            static_cast<unsigned char>(std::min(std::max(value, 0), 255)) };
                    bmp_file.write(reinterpret_cast<char*>(pixel), PIXEL_SIZE);
                }
            }
        }

        else
        { 
            //для разукраски класстеров
            for (int i = int(x_size) - 1; i >= 0; --i)
            {
                for (int j = 0; j < int(y_size); ++j)
                {
                    unsigned char pixel[3] = { static_cast<unsigned char>(colors[i][j].r), 
                                               static_cast<unsigned char>(colors[i][j].g), 
                                               static_cast<unsigned char>(colors[i][j].b) };
                    bmp_file.write(reinterpret_cast<char*>(pixel), PIXEL_SIZE);
                }
            }
        }

        bmp_file.close();
        return SUCCESS_BMP_WRITE;
    }

    int Init_field::read_bmp(Gauss_program::Init_field* field, char* BMP_file_name)
    //212-Chernyshova-Svetlana
    //reads bmp
    {
        int x_size_int, y_size_int;
        
        std::ifstream bmpFile(BMP_file_name, std::ios::binary);
        if (!bmpFile) 
        {
            return BMP_FILE_FOR_READING_NOT_FOUND;
        }
        // Читаем заголовок BMP
        unsigned char header[54];
        bmpFile.read(reinterpret_cast<char*>(header), 54);
        // Получаем ширину и высоту изображения
        x_size_int = modul(header[18] | (header[19] << 8) | (header[20] << 16) | (header[21] << 24));
        y_size_int = modul(header[22] | (header[23] << 8) | (header[24] << 16) | (header[25] << 24));
        x_size = size_t(x_size_int);
        y_size = size_t(y_size_int);
        // Инициализируем новое поле
        //delete field_arr;
        //field_arr = new int[x_size_int * y_size_int];
        //if (field_arr == nullptr) { return FAIL_INIT; }
        field->initialize();
        
        // Читаем данные пикселей
        /*
        for (int y = y_size_int - 1; y >= 0; --y) 
        { // BMP хранит данные снизу вверх
            for (int x = x_size_int - 1; x >= 0; --x)
            {
        */

        for (int y = 0; y < y_size_int; ++y)
        { // BMP хранит данные снизу вверх
            for (int x = 0; x < x_size_int; ++x)
            {
                unsigned char color = bmpFile.get(); // Читаем B
                bmpFile.get(); // Читаем G
                bmpFile.get(); // Читаем R
                double value = color; // Цвет в высоту
                field_arr[y * y_size_int + x] = value; // Обновляем матрицу значений
            }
            bmpFile.ignore((4 - (x_size_int * 3) % 4) % 4); // Пропускаем паддинг
        }
        bmpFile.close();
        return SUCCESS_BMP_READ;
    }


// Destructor
    Init_field::~Init_field()
    {
//            delete[] field_arr;

        if (field_arr && initialization_worked == 1)
        {
            delete[] field_arr;
            field_arr = nullptr;
        }

        else if (field_arr)
        {
            //delete field_arr;
            field_arr = nullptr;
        }
        
    }


    //class Interface
    int Interface::command_receiving_place(FILE ** input_stream_name)
    //212-Chernyshova-Svetlana
    //gets the place of commands
    {
        //FILE* input_stream_name = nullptr;
        char input_file_name[50];
        int the_input_way = 0;
        int flag1, flag2;
        
        //printf("112-Chernyshova-Svetlana-Gauss.\n");
        //printf("We are going to make a field of gaussians.\n");
        printf("How do you want to input the commands?\n1: Concole\n2: Text file\n");

        flag1 = scanf("%d", &the_input_way);
        getchar(); // вытаскиваем символ '\n' из буфера
        
        if(flag1 != 1) 
        {
            printf("I don't know this command.\n");
            return FAIL_command_receiving_place;
        }
        if (the_input_way == 1) 
        {
            *input_stream_name = stdin; //??? ok?
        }
        else if (the_input_way == 2) 
        {
            printf("Please enter the file name :\n");
            //printf("2\n");

            flag2 = scanf("%s", input_file_name);
            getchar(); // вытаскиваем символ '\n' из буфера

            if (flag2 != 1) 
            {
                //printf("3\n");
                printf("Can't understand this file name :/\n");
                return FAIL_command_receiving_place;
            }
            *input_stream_name = fopen(input_file_name, "r");
            if (!(*input_stream_name))
            {
                //printf("4\n");
                printf("No such file in the directory.\n");
                //fclose(input_stream_name);
                //printf("5\n");
                return FAIL_command_receiving_place;
            }
        }
        else
        {
            printf("I don't know this command\n");
            return FAIL_command_receiving_place;
        }
        //printf("1 command receiving place\n");
        printf("Understood the command receiving place.\n");
        return SUCCESS_command_receiving_place;
    }
/*
    int Interface::read_commands(FILE * input)
    {

    }
*/
/*
    Interface::Interface()
    {

    }
*/

    Interface::~Interface()
    {
        if (field)
        {
            delete field;
        }
    }

    int Interface::understand_command(char * get_command_from)
    //212-Chernyshova-Svetlana
    //decrypts commands
    {
        //int i;
        int feedback = -1;
        const char * sep = " ";
        char * saveptr;
        char istr[LEN_SMALL];
        int hight, mu_x, mu_y, s_x, s_y;
        size_t size_x,size_y;
        char GNU_file_name_arr[MAX_AMOUNT], GNU_data_file_name_arr[MAX_AMOUNT];
        //char *GNU_file_name;
        //char *GNU_data_file_name;
        char BMP_created_file_name_arr[MAX_AMOUNT], BMP_read_file_name_arr[MAX_AMOUNT];
        //char *BMP_created_file_name;
        //char *BMP_read_file_name;
        int cut_value;
        int component_min_size;
        int k_kmeans;
        int k_kmCores, cores_kmCores;

        char* tmp; //для strtok_r
        //printf("14\n");
        //fscanf(input,)
        sscanf(strtok_r(get_command_from, sep, &saveptr), "%s", istr);
        //if (!istr) // строка файла пуста - не будем проверять
        //printf("12\n");
        printf("%s\n", istr);
        //printf("gev gev gev\n");
//GSS
        if (istr[0] == 'G' && istr[1] == 'S' && istr[2] == 'S')
        {
            printf("GNU-check-1\n");
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%d", &hight) == 1))
            {
                printf("GNU-check-2\n");
                hight = interface_Gauss_DEF_hight;
            }
            printf("1\n");
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%d", &mu_x) == 1))
            {
                printf("GNU-check-3\n");
                mu_x = interface_Gauss_DEF_mu_x;
            }
            printf("2\n");
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%d", &mu_y) == 1))
            {
                printf("GNU-check-4\n");
                mu_y = interface_Gauss_DEF_mu_y;
            }
            printf("3\n");
            tmp = strtok_r(nullptr, sep, &saveptr);
            cout<<"1st "<<tmp<<endl;
            if ( tmp == nullptr || tmp[0] == '\0' || tmp[0] == '\n' || tmp[0] == '\r' || !(sscanf(tmp, "%d", &s_x) == 1))
            {
                printf("GNU-check-5\n");
                s_x = interface_Gauss_DEF_sigma_x;
            }
            cout<<"2nd "<<tmp<<endl;
            printf("4\n");
            cout<<"3rd "<<tmp<<endl;
            tmp = strtok_r(nullptr, sep, &saveptr);
            //if ()
            cout<<"4th "<<endl;
            cout<<tmp<<endl;
            printf("666666\n");
            if ( tmp == nullptr || tmp[0] == '\0' || tmp[0] == '\n' || tmp[0] == '\r' || !(sscanf(tmp, "%d", &s_y) == 1))
            {
                printf("GNU-check-6\n");
                printf("interface_Gauss_DEF_sigma_y : %d", interface_Gauss_DEF_sigma_y);
                printf("s_y : %d", s_y);

                s_y = interface_Gauss_DEF_sigma_y;

                printf("interface_Gauss_DEF_sigma_y : %d", interface_Gauss_DEF_sigma_y);
                printf("s_y : %d", s_y);
            }
            printf("5\n");
            //printf("check-1\n");
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            //printf("check-2\n");
            feedback = control_obj.make_gauss(hight, mu_x, mu_y, s_x, s_y);
            //printf("check-3\n");
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
            //printf("check-4\n");
        } 
//INIT
        else if (istr[0] == 'I' && istr[1] == 'N' && istr[2] == 'I' && istr[3] == 'T')
        {
            //printf("Try initializing\n");
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%zu", &size_x) == 1))
            {
                size_x = interface_Field_DEF_x_size;
            }
            else if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%zu", &size_y) == 1))
            {
                size_y = interface_Field_DEF_y_size;
            }
            //printf("Trying\n");
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            feedback = control_obj.init_f(size_x, size_y, field); // проблема с тем чтобы вернуть указатель на поле
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
            //printf("feedback\n");
        } 
//GNU
        else if (istr[0] == 'G' && istr[1] == 'N' && istr[2] == 'U')
        {
            //printf("error\n");
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%s", GNU_data_file_name_arr) == 1))
            {
                //strcpy()
                //strcpy(GNU_data_file_name, interface_Gnuplot_data_filename);
                strcpy(GNU_data_file_name_arr, interface_Gnuplot_data_filename);
            }
            else if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%s", GNU_file_name_arr) == 1))
            {
                //strcpy(GNU_file_name, interface_Gnuplot_commands_filename);
                strcpy(GNU_file_name_arr, interface_Gnuplot_commands_filename);
            }
            //printf("going to make gnuplot\n");
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            feedback = control_obj.make_gnuplot_file(field, GNU_data_file_name_arr, GNU_file_name_arr);
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
            //printf("made gnuplot\n");
        }
//GEN
        else if (istr[0] == 'G' && istr[1] == 'E' && istr[2] == 'N')
        {
            //printf("Trying to generate\n");
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            feedback = control_obj.generate_field_with_gausses(field);
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
//s
/*
            size_t p=0,q=0;
            size_t nn_x;
            nn_x = field->get_x_size();
            size_t nn_y;
            nn_y = field->get_y_size();

            for (p = 0; p < nn_x; p++)
            {
                for(q = 0; q < nn_y; q++)
                {
                    printf("%d ", field->get_field_value(p,q));
                }
                printf("\n");
            }
*/
//e
        }


// Create BMP
// возможно надо подправить имя файла!!! (расширение) -- не надо
        else if (istr[0] == 'C' && istr[1] == 'R' && istr[2] == 'B' && istr[3] == 'M' && istr[4] == 'P')
        {
            //printf("CR_BMP-check-0\n");

//ДЛЯ чтения sscanf-ом НУЖНО как было массивом char-ов имя файла задавать......
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%s", BMP_created_file_name_arr) == 1))
            {
                //BMP_created_file_name = interface_BMP_creation_filename; // вообще говоря это const char и оно может ругаться
                //printf("CR_BMP-check-1\n");
                char *arr;
                //printf("CR_BMP-check-2\n");
                strcpy(arr, interface_BMP_creation_filename);
                //printf("CR_BMP-check-3\n");
                std::string arr_str = string(arr);
                //printf("CR_BMP-check-4\n");
                //BMP_created_file_name = arr_str.c_str(); 
                strcpy(BMP_created_file_name_arr, arr_str.c_str()); 
                //printf("CR_BMP-check-5\n");
            }
// в процессе!!
            //printf("CR_BMP-check-6\n");
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            //printf("CR_BMP-check-7\n");
            feedback = control_obj.make_bmp_file(field, BMP_created_file_name_arr);
            //printf("CR_BMP-check-7\n");
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
            //printf("CR_BMP-check-8\n");
        }

// Read BMP
        else if (istr[0] == 'R' && istr[1] == 'D' && istr[2] == 'B' && istr[3] == 'M' && istr[4] == 'P')
        {
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%s", BMP_read_file_name_arr) == 1))
            {
                strcpy(BMP_read_file_name_arr, interface_BMP_reading_filename);
            }
// в процессе!!
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            feedback = control_obj.read_bmp_file(field, BMP_read_file_name_arr);
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
        }
// BIN
        else if (istr[0] == 'B' && istr[1] == 'I' && istr[2] == 'N')
        {
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%d", &cut_value) == 1))
            {
                cut_value = interface_BIN_hight;
            }
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            feedback = control_obj.bin_cutting(field, cut_value);
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
        }
//WAVE
        else if (istr[0] == 'W' && istr[1] == 'A' && istr[2] == 'V' && istr[3] == 'E')
        {
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%d", &component_min_size) == 1))
            {
                component_min_size = interface_WAVE_min_component_size;
            }
            //printf("start_wave\n");
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            feedback = control_obj.wave_command(component_min_size);
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
        }
//KMEANS
        else if (istr[0] == 'K' && istr[1] == 'M' && istr[2] == 'E' && istr[3] == 'A' && istr[4] == 'N' && istr[5] == 'S')
        {
            //printf("before kmeans check\n");
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%d", &k_kmeans) == 1))
            {
                printf("kmeans-check-1\n");
                k_kmeans = interface_KMEANS_amount_of_dots;
            }
            //printf("kmeans-check-2\n");
            interface_logging.log("The interface has given control to the Control (class) and sent the command " + std::string(istr) + " to the Control. Waiting for an answer.\n");
            //printf("kmeans-check-3\n");
            feedback = control_obj.kMeans_command(k_kmeans);
            //printf("kmeans-check-4\n");
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
            //printf("kmeans-check-5\n");
        }
//KMEANS WITH CORES
        else if (istr[0] == 'K' && istr[1] == 'M' && istr[2] == 'C' && istr[3] == 'O' && istr[4] == 'R' && istr[5] == 'E')
        {
            if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%d", &k_kmCores) == 1))
            {
                k_kmCores = interface_KMCORE_amount_of_cores;
            }
            else if ( !(sscanf(strtok_r(nullptr, sep, &saveptr), "%d", &cores_kmCores) == 1))
            {
                cores_kmCores = interface_KMCORE_amount_of_dots_in_cores;
            }
            interface_logging.log("The interface has given control to the Control (class) and sent the command" + std::string(istr) + "to the Control. Waiting for an answer.\n");
            feedback = control_obj.KMCORES_command(k_kmCores, cores_kmCores);
            interface_logging.log("The interface has returned the control and received a response from the Control.\n");
        }


        return feedback;
    }

    
    std::string Interface::trim(const std::string& str)
    { 
        // Функция, чтобы убирать пробелы в начале и конце строки
        //printf("111\n");
        size_t first = str.find_first_not_of(" \n\r");
        //std::cout<<first<<std::endl;
        //printf("00\n");
        if(first == std::string::npos)
        {
            return "";
        }
        //printf("(((\n");
        size_t last = str.find_last_not_of(" \n\r");// короче надо чтобы оно перенос строки не захватывало
        //std::cout<<last<<std::endl;

        //size_t last
        //if (last == std::string::npos) return "";
        if (first == last) return str.substr(first); //подстрока начиная с first
        return str.substr(first, (last - first + 1));
        //return str.substr(first, (last - first + 1));
    }


    //void Interface::config_read(std::string interface_log_filename)
    void Interface::config_read()
    {
        char arr[MAX_AMOUNT];
        std::cout << "Write the name of the configuration file :" << std::endl;
        std::cin >> interface_log_filename; // поле класса
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        //char config_filename_arr[MAX_AMOUNT];

        //FILE *config = fopen(interface_log_filename.c_str(), "r"); //FILE*
        std::ifstream config(interface_log_filename, std::ios::in); //FILE*

        if( !config.is_open())
        {
            std::cerr << "Failed to open the configuration file : " << interface_log_filename << std::endl;
            //return -1;
        }
        std::cout << "Succesfully opened file : " << interface_log_filename << std::endl;

        std::string line;
        //printf("5\n");
        while (std::getline(config, line))
        {
            //printf("6\n");
            std::istringstream iss(line); // iss делается местом ввода данных
            std::string key, value;

            if(std::getline(iss, key, '=') && std::getline(iss, value, '\n')) //
            {
                //printf("7\n");
                //std::cout << "{" << key << "}" << std::endl;
                key = trim(key);
                //std::cout << "{" << key << "}" << std::endl;

                //std::cout << "{" << value << "}" << std::endl;
                value = trim(value);
                //std::cout << "{" << value << "}" << std::endl;
                //printf("Before log_interface check\n");
// Log_Interface
                if (key == "Log_Interface")
                {
                    //printf("After log_interface check\n");
                    //std::cout << "." << value << "." << std::endl;
                    if (value == "ON")
                    {
                        //printf("After ON check\n");
                        std::string log_filename = "log_interface_prog_var.txt";
                        std::getline(config, line);
                        std::istringstream iss_filename(line);
                        if (std::getline(iss_filename, key, '=') && std::getline(iss_filename, value))
                        {
                            //printf("8\n");
                            key = trim(key);
                            value = trim(value);
                            if(key == "Log_Interface_filename")
                            {
                                log_filename = value;
                            }
                        }
                        interface_logging.enable_interface_logging(log_filename);
                        std::cout << "Interface logging enabled with file : " << log_filename << std::endl;
                    }
                }
// Log_Control

                else if (key == "Log_Control")
                {
                    if(value == "ON")
                    {
                        std::string log_filename = "log_control_prog_var.txt";
                        std::getline(config, line);
                        std::istringstream iss_filename(line);
                        if (std::getline(iss_filename, key, '=') && std::getline(iss_filename, value))
                        {
                            key = trim(key);
                            value = trim(value);
                            if(key == "Log_Control_filename")
                            {
                                log_filename = value;
                            }
                        }
                        control_obj.control_logging.enable_control_logging(log_filename);
                        std::cout << "Interface logging enabled with file: " << log_filename << std::endl;
                    }
                }


// Batch_file_name
                
                else if (key == "Batch_file")
                {
                    if (value == "ON")
                    {
                        interface_Batch_filename = "Batch_file.txt";
                        std::getline(config, line);
                        std::istringstream iss_filename(line);

                        if (std::getline(iss_filename, key, '=') && std::getline(iss_filename, value))
                        {
                            key = trim(key);
                            value = trim(value);
                            if (key == "Batch_filename")
                            {
                                interface_Batch_filename = value;
                            }
                        }
                    }
                }

// Field_init
                else if (key == "Field_DEF_x_size")
                {
                    interface_Field_DEF_x_size = stoi(value);
                }
                else if (key == "Field_DEF_y_size")
                {
                    interface_Field_DEF_y_size = stoi(value);
                }

// Gauss_init
                else if (key == "Gauss_DEF_hight")
                {
                    interface_Gauss_DEF_hight = stoi(value);
                }
                else if (key == "Gauss_DEF_mu_x")
                {
                    interface_Gauss_DEF_mu_x = stoi(value);
                }
                else if (key == "Gauss_DEF_mu_y")
                {
                    interface_Gauss_DEF_mu_y = stoi(value);
                }
                else if (key == "Gauss_DEF_sigma_x")
                {
                    interface_Gauss_DEF_sigma_x = stoi(value);
                }
                else if (key == "Gauss_DEF_sigma_y")
                {
                    interface_Gauss_DEF_sigma_y = stoi(value);
                }
// BMP_creation
                else if (key == "BMP_creation_filename")
                {
                    //printf("before BMP creation check\n");
                    interface_BMP_creation_filename = value.c_str();
                    //printf("after BMP creation check\n");
                }
// BMP_reading
                else if (key == "BMP_reading_filename")
                {
                    //printf("\nbefore BMP reading check\n");
                    strcpy(arr, value.c_str());
                    interface_BMP_reading_filename = arr;
                    //printf("\nafter BMP reading check\n");
                }
// Gnuplot
                else if (key == "Gnuplot_data_filename")
                {
                    //printf("\nbefore gnuplot check\n");
                    strcpy(arr, value.data()); //не может ли сломаться от того что я снова в ту же arr пытаюсь что-то новое копировать?..))
                    interface_Gnuplot_data_filename = arr;
                    //printf("after gnuplot check\n");
                }
                else if (key == "Gnuplot_commands_filename")
                {
                    strcpy(arr, value.data());
                    interface_Gnuplot_commands_filename = arr;
                }
// BIN
                else if (key == "BIN_hight")
                {
                    interface_BIN_hight = stoi(value);
                }
// WAVE
                else if (key == "WAVE_min_component_size")
                {
                    interface_WAVE_min_component_size = stoi(value);
                }
// KMEANS
                else if (key == "KMEANS_amount_of_dots")
                {
                    interface_KMEANS_amount_of_dots = stoi(value);
                }
// KMEANS_CORES
                else if (key == "KMCORE_amount_of_cores")
                {
                    interface_KMCORE_amount_of_cores = stoi(value);
                }
                else if (key == "KMCORE_amount_of_dots_in_cores")
                {
                    interface_KMCORE_amount_of_dots_in_cores = stoi(value);
                }
                
// вроде все значения
            }
        }
    }

    //class Control
  
    int Control::init_f (size_t n_x, size_t n_y, Init_field * field)
    //212-Chernyshova-Svetlana
    //CONTROL: init_field
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command INIT.\n");
        int feedback;
        //int i,j;
        //Init_field *Field; //здесь нельзя ссылку тк ее изменение меняет то что по ней расположено, а мы хотим сохранить сам адрес
        //printf("0000\n");
        if (bmp_reading_initialization == 1)
        {
            control_logging.logs_of_control("The field already exsists, it was read from a bmp file, there is no opportunity to make one more with initialization. The command skipped.\nThe control is given back to the Interface.\n");
            return FIELD_INIT_TRY_AFTER_BMP_READ;
        }
        if (field_initialization == 0 && bmp_reading_initialization == 0)
        {
            ///printf("1111\n");
            // тело // проверить эту *
            //Field = new Init_field(n_x, n_y);

            field->set_x_size(n_x);
            field->set_y_size(n_y);

            feedback = field->initialize();

            //flag
            field_initialization = 1;
            control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
            return feedback;
        }
        else 
        {
            control_logging.logs_of_control("The field already exsists, there is no opportunity to make one more. The command skipped.\nThe control is given back to the Interface.\n");
            //WARNING
            return SECOND_FIELD_INITIALIZATION;
        }
        //return SUCCESS_INIT;
        //return Field; //адрес // ругается потому что нет конструктора копирования для этого класса
    }
    
    int Control::generate_field_with_gausses(Init_field* field) //объект поля будет в интерфейсе //
    //212-Chernyshova-Svetlana
    //CONTROL: gen
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command GEN.\n");
        if (field_initialization != 1)
        {
            control_logging.logs_of_control("Generation, gnuplot and bmp can't be complete without a field. The field has't been initialized. The command skipped.\nThe control is given back to the Interface.\n");
            //WARNING
            return ERROR_UNINITIALIZED_FIELD;
        }

        l.generate_field(field);
        field_generation = 1;
        control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
        return SUCCESS_GEN;
    }

    int Control::make_gnuplot_file(Init_field* field, char * GNU_data_file_name, char * GNU_file_name) 
    //212-Chernyshova-Svetlana
    //CONTROL: gnuplot
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command GNU.\n");
        int feedback;
        feedback = field->gnuplot(GNU_data_file_name, GNU_file_name);
        //printf("000\n");
        if (field_initialization == 0) 
        {
            control_logging.logs_of_control("Generation, gnuplot and bmp can't be complete without a field. The field has't been initialized. The command skipped.\nThe control is given back to the Interface.\n");
            return ERROR_UNINITIALIZED_FIELD;
        }
        if (field_generation == 0 && bmp_reading_initialization == 0)
        {
            control_logging.logs_of_control("Can't make gnuplot because there are no gausses on the field. The command skipped.\nThe control is given back to the Interface.\n");
            return GNU_fail_not_generated_field;
        }
        control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
        return feedback;
    }


    int Control::make_bmp_file(Gauss_program::Init_field* field, const char * BMP_file_name)
    //212-Chernyshova-Svetlana
    //CONTROL: make bmp
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command CRBMP.\n");
        int feedback;
        if (field_initialization == 0) 
        {
            control_logging.logs_of_control("Generation, gnuplot and bmp can't be complete without a field. The field has't been initialized. The command skipped.\nThe control is given back to the Interface.\n");
            return ERROR_UNINITIALIZED_FIELD;
        }

        feedback = field->make_bmp(BMP_file_name);
        control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
        return feedback;
    }


    int Control::read_bmp_file(Gauss_program::Init_field* field, char * BMP_file_name)
    //212-Chernyshova-Svetlana
    //CONTROL: read bmp
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command RDBMP.\n");
        int feedback;
        if (field_initialization == 1)
        {
            control_logging.logs_of_control("The field already exsists, there is no opportunity to make one more. The command skipped.\nThe control is given back to the Interface.\n");
            return SECOND_FIELD_INITIALIZATION;
        }
        if (amount_of_gauss != 0)
        {
            control_logging.logs_of_control("The gausses has already been added. Not able to use bmp reading for field initialization. The command skipped.\nThe control is given back to the Interface.\n");
            return NO_bmp_reading_after_gausses;
        }
        //printf("99\n");
        feedback = field->read_bmp(field, BMP_file_name);

        if (feedback == SUCCESS_BMP_READ)
        {
            bmp_reading_initialization = 1;
            field_initialization = 1;
        }
        control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
        return feedback; 
        ////+ сохранить момент инициализации поля!!!!
    }

    int Control::bin_cutting(Gauss_program::Init_field* field, int cut_value)
    //212-Chernyshova-Svetlana
    //CONTROL: bin
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command BIN.\n");
        int feedback;
        if (field_generation == 0 && bmp_reading_initialization == 0)
        {
            control_logging.logs_of_control("There is no any field yet, so cutting is impossible. The command skipped.\nThe control is given back to the Interface.\n");
            return NO_FIELD_FOR_CUTTING;
        }
        feedback = comp_obj.bin(field, cut_value);
        control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
        return feedback;
    }

    int Control::wave_command(int component_min_size)
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command WAVE.\n");
        int feedback;
        //printf("wave_control\n");
        if (field_generation == 0 && bmp_reading_initialization == 0)
        {
            control_logging.logs_of_control("There is no any field yet, so wave command is impossible. The command skipped.\nThe control is given back to the Interface.\n");
            return NO_FIELD_FOR_WAVE;
        }
        feedback = comp_obj.wave_start(component_min_size);
        control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
        return feedback;
    }

    int Control::kMeans_command(int k)
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command KMEANS.\n");
        int feedback;
        // Алгоритм kMeans
        if(field_generation == 0 && bmp_reading_initialization == 0)
        {
            control_logging.logs_of_control("There is no any field yet, so kMeans command is impossible. The command skipped.\nThe control is given back to the Interface.\n");
            return NO_FIELD_FOR_KMEANS;
        }
        //server.log("Algotitgm KMEANS for the k =" + std::to_string(k) + " is launched");
        feedback = comp_obj.kMeans(k);
        //server.log("Algorithm KMEANS for the k =" + std::to_string(k) + " is completed");
        control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
        return feedback;
    }

    int Control::KMCORES_command(int k, int p)
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command KMCORES.\n");
        int feedback;
        // Алгоритм kMeans
        if(field_generation == 0 && bmp_reading_initialization == 0)
        {
            control_logging.logs_of_control("There is no any field yet, so kmCores command is impossible. The command skipped.\nThe control is given back to the Interface.\n");
            return NO_FIELD_FOR_KMCORES;
        }
        //server.log("Algotitgm KMCORES for k = " + std::to_string(k) + " and p = " + std::to_string(p) + " is launched");
        feedback = comp_obj.KMCORES(k,p);
        //server.log("Algorithm KMCORES for the k = " + std::to_string(k) + " and p = " + std::to_string(p) + " is completed");
        control_logging.logs_of_control("The command is completed, the report will be in the Interface.\nThe control is given back to the Interface.\n");
        return feedback;
    }

    int Control::make_gauss(int h, int x, int y, int s_x, int s_y) // нужна ссылка на след элемент списка)
    //212-Chernyshova-Svetlana
    //CONTROL: add gauss to the list
    {
        control_logging.logs_of_control("Class Control received control from Interface and received command GSS.\n");
        if (bmp_reading_initialization == 1)
        {
            control_logging.logs_of_control("The field already exsists, it was read from a bmp file, there is no opportunity to make if with gausses, so we are not going to remember them. The command skipped.\nThe control is given back to the Interface.\n");
            return GAUSS_ADD_TRY_AFTER_BMP_READ;
        }
    // создали Гаусс и добавили в список
        Gauss g(h,x,y,s_x,s_y);
        l.push_back(g); //контрол не сам делает, уходит в лист
        amount_of_gauss += 1;
        //printf("%d\n",amount_of_gauss);
        control_logging.logs_of_control("Gauss was successfully created.\nThe control is given back to the Interface.\n");
        return SUCCESS_GSS;
    }

/*
    void Control::logs(char* control_log_filename, int feedback)
    {

        log_control_file_obj.feedback_printing(control_log_filename, feedback);
    }
*/

//flags
    int Control::field_initialization = 0; // магия работает
    int Control::field_generation = 0;
    int Control::bmp_reading_initialization = 0;

/*
    Control::~Control()
    {

    }
*/

    //class Components

// Constructor : !!!!!!

// Destructor : !!!!!!

//functions:
    int Components::bin(Gauss_program::Init_field* field, int cut_value)
    //212-Chernyshova-Svetlana
    //bin realization
    {
        if (cut_value < 0 || cut_value > 255)
        {
            return IMPOSSIBLE_CUT_VALUE;
        }
        int feedback;
        int x, y;
        int field_value;
        int x_size_of_slice_matrix, y_size_of_slice_matrix;
        //int * slice_matrix = nullptr;
        //char slice_bmp_file_name[] = "Bin_slice_number_" + std::to_string(amount_of_bin_slices) + ".bmp\0";
        ////const std::string& slice_bmp_file_name_str = "Bin_slice_number_" + std::to_string(amount_of_bin_slices) + ".bmp";
        ////char * slice_bmp_file_name = slice_bmp_file_name_str.c_str();
        //const char * slice_bmp_file_name = 
        x_size_of_slice_matrix = int(field->get_x_size());
        y_size_of_slice_matrix = int(field->get_y_size());

        //printf("you\n");

        Init_field* slice = new Init_field(); //утеклооо
        slice->set_x_size(size_t(x_size_of_slice_matrix));
        slice->set_y_size(size_t(y_size_of_slice_matrix));
        feedback = slice->initialize_with_zeros();
        //slice_matrix = new int[int(x_size_of_slice_matrix * y_size_of_slice_matrix)];
        
        if (feedback != SUCCESS_INIT)
        {
            return FAIL_bin_matrix_init;
        }

        for (x = 0; x < x_size_of_slice_matrix; x++)
        {
            for (y = 0; y < y_size_of_slice_matrix; y++)
            {
                ///printf("get_field_value(%d,%d) : %d\n",x,y,field->get_field_value(x,y));
                field_value = field->get_field_value(x,y);
                if (field_value > cut_value)
                {
                    slice->set_definite_field_value(x, y, 255);
                }
                else
                {
                    slice->set_definite_field_value(x, y, 0);
                }
            }
        }

        bin_slice_list.matrix_list_push_back(slice);

        amount_of_bin_slices += 1;
        //printf("amount_of_bin_slices : %d\n", amount_of_bin_slices);
        //slice_bmp_file_name = ("Bin_slice_number_.txt");
        //*slice_bmp_file_name = ("Bin_slice_number_%d.txt", amount_of_bin_slices);
        slice->make_bmp(("Bin_slice_number_" + std::to_string(amount_of_bin_slices) + ".bmp").c_str());
        ////slice->make_bmp(slice_bmp_file_name);
        return BIN_SUCCESS;

    }

    int Components::wave_start(int component_min_size)
    {
        int feedback;
        //printf("wave_start in Comp\n");
        Matrix_Node* bin_matrix_node = bin_slice_list.matrix_list_first_node; //присваивается голова всего списка // указатель типа Node
        // в строчке выше сработало через точку, тк есть объект типа List
        //printf("99\n");
        int bin_slice_number = 1;
        if (bin_matrix_node == bin_matrix_node->next)
        {
            feedback = wave(bin_matrix_node->value, component_min_size, bin_slice_number);
            ///printf("feedback in wave_start : %d \n",feedback);
            if (feedback != SUCCESS_WAVE)
            {
                return feedback;
            }
            return SUCCESS_WAVE;
        }
        while (bin_matrix_node != nullptr)
        {
            printf("bin_matrix_node != nullptr\n");
            std::cout<<"bin_matrix_node : "<<bin_matrix_node<<"\n";
            feedback = wave(bin_matrix_node->value, component_min_size, bin_slice_number);
            ///printf("feedback in wave_start : %d \n",feedback);
            if (feedback != SUCCESS_WAVE)
            {
                return feedback;
            }
            //feedback = this->wave(bin_matrix_node.value);
            if (bin_matrix_node != bin_matrix_node->next) // равны когда 1 элем в списке
            {
                bin_matrix_node = bin_matrix_node->next; // присваиваем адрес следующего узла, хранящийся в next-е
                //std::cout<<"bin_matrix_node : next : "<<bin_matrix_node<<"\n";
                bin_slice_number += 1;
            }

        }
        return SUCCESS_WAVE;
    }


    int Components::wave(Gauss_program::Init_field* bin_matrix, int component_min_size, int bin_slice_number)
    { 
        // Функция запуска алгоритма wave и записи соотв. компонент в вектор
        //Component Componenta(length, width); //новая матрица: компонента размера length*width
        //std::string& filename
        ////char * bmp_write_file_name;
        ///printf("pum pum\n");
        int feedback;
        int x_size_of_slice_matrix = int(bin_matrix->get_x_size());
        int y_size_of_slice_matrix = int(bin_matrix->get_y_size());

        Init_field* wave_slice = new Init_field();
        wave_slice->set_x_size(size_t(x_size_of_slice_matrix));
        wave_slice->set_y_size(size_t(y_size_of_slice_matrix));
        feedback = wave_slice->initialize_with_zeros();
        if (!(feedback == SUCCESS_INIT))
        {
            return FAIL_creating_wave_slice;
        }

        Init_field* check_comp_matrix = new Init_field(); //матрица для отслеживания какие точки уже добавлены в некоторую компоненту
        check_comp_matrix->set_x_size(size_t(x_size_of_slice_matrix));
        check_comp_matrix->set_y_size(size_t(y_size_of_slice_matrix));
        feedback = check_comp_matrix->initialize_with_zeros();
        if (!(feedback == SUCCESS_INIT))
        {
            return FAIL_creating_wave_slice;
        }

        Init_field* matrix_without_small_components = new Init_field();
        matrix_without_small_components->set_x_size(size_t(x_size_of_slice_matrix));
        matrix_without_small_components->set_y_size(size_t(y_size_of_slice_matrix));
        feedback = matrix_without_small_components->initialize_with_zeros();
        if (!(feedback == SUCCESS_INIT))
        {
            return FAIL_creating_wave_slice;
        }

        for(int i = x_size_of_slice_matrix - 1; i >= 0; --i)
        {
            for(int j = 0; j < y_size_of_slice_matrix; ++j)
            {
                ////int count_pixels_in_component = 0;
                //printf("1\n");
                if (check_comp_matrix->get_field_value(i,j) == 0 && bin_matrix->get_field_value(i,j) == 255)
                {
                    //printf("HERE\n");
                    ////////printf("inc : %d\n", inc(bin_matrix, check_comp_matrix, wave_slice, i, j, 1));

                    //feedback = wave_slice->make_bmp(("Comp" + std::to_string(amount_of_components) + ".bmp").c_str());
                    /*
                    if (feedback != SUCCESS_BMP_WRITE)
                    {
                        return WAVE_Error_writing_bmp;
                    }
                    */
                    if(inc(bin_matrix, check_comp_matrix, wave_slice, i, j, 1) > component_min_size)
                    {
                        //printf("HERE I AM\n");
                        //если компонента получилась большой, сохраняем ее и делаем новую матрицу для след компоненты
                        amount_of_components += 1;
                        //bmp_write_file_name = "Comp" + std::to_string(amount_of_components) + ".bmp"; 
                        //("Bin_slice_number_" + std::to_string(amount_of_bin_slices) + ".bmp").c_str()
                        
                        feedback = wave_slice->make_bmp(("Bin_" + std::to_string(bin_slice_number) + "_comp_" + std::to_string(amount_of_components) + ".bmp").c_str());
                        
                        if (feedback != SUCCESS_BMP_WRITE)
                        {
                            return WAVE_Error_writing_bmp;
                        }
                        components_slice_list.matrix_list_push_back(wave_slice);
                        //std::cout<<"wave_slice : "<<wave_slice<<"\n";
                        //std::cout<<"wave_slice из листа : "<<components_slice_list.matrix_list_first_node->value<<"\n";
                        //std::cout<<"wave_slice из листа next : "<<components_slice_list.matrix_list_first_node->next->value<<"\n";
                        ////////wave_slice = nullptr;

                        wave_slice = new Init_field();
                        wave_slice->set_x_size(size_t(x_size_of_slice_matrix));
                        wave_slice->set_y_size(size_t(y_size_of_slice_matrix));
                        feedback = wave_slice->initialize_with_zeros();
                        if (!(feedback == SUCCESS_INIT))
                        {
                            return FAIL_creating_wave_slice;
                        }

                    }
                }
            }
        }

        Matrix_Node* components_matrix_node = components_slice_list.matrix_list_first_node; //присваивается голова всего списка // указатель типа Node
        ///std::cout<<"wave_slice из листа позднее : "<<components_matrix_node->next->value<<"\n";

        Init_field *ptr;
        int comp_value;
        if (components_matrix_node == components_matrix_node->next) // один элемент в списке
        {
            for(int i = 0; i < x_size_of_slice_matrix; ++i)
            {
                for(int j = 0; j < y_size_of_slice_matrix; ++j)
                {
                    ptr = components_matrix_node->value;
                
                    comp_value = ptr->get_field_value(i, j);
                    if (comp_value != 0)
                    {
                        matrix_without_small_components->set_definite_field_value(i, j, 254);
                    }
                }
            }
        }
        else
        {
            while (components_matrix_node != nullptr)
            {
                for(int i = x_size_of_slice_matrix - 1; i >= 0; --i)
                {
                    for(int j = 0; j < y_size_of_slice_matrix; ++j)
                    {
                        ptr = components_matrix_node->value;
                        comp_value = ptr->get_field_value(i, j);                        
                        if (comp_value != 0)
                        {
                            matrix_without_small_components->set_definite_field_value(i, j, 254);
                        }
                    }
                }
                components_matrix_node = components_matrix_node->next; 
            }
        }

        feedback = matrix_without_small_components->make_bmp(("Bin_" + std::to_string(bin_slice_number) + "_comp_all.bmp").c_str());
        if (feedback != SUCCESS_BMP_WRITE)
        {
            return WAVE_Error_writing_bmp;
        }


/*
        for(int i = 0; i < amount_of_components; i++)
        {
            //Field compole(length, width);
            //Init_field* all_components = new Init_field();    
            //compole.matrix = components[i].comp;
            components_slice_list.bmp_write("Comp" + std::to_strin1) + ".bmp");
        }
*/
        
        return SUCCESS_WAVE;
    }

        
    int Components::inc(Gauss_program::Init_field* bin_matrix, Gauss_program::Init_field* check_comp_matrix, Gauss_program::Init_field* component, int x, int y, int k)
    { 
    // 212-Чернышова-Светлана
    // Сама функция wave
    // bin_matrix - матрица бинарного среза. По ней проверяем, надо ли вообще рассматривать конкретную координату или нет. Не изменяем
    // check_comp_matrix - матрица со значениями 0 и 255. Хранит в себе информацию о том, отнесли ли уже (ранее) данную координату к какой-то компоненте
    // component - сама матрица компоненты. Таких будет список. По ней идет сам алгоритм wave
    // k не изменяется
        //int number_wave = 1; // номер который записывается в волне
        //int static count_pixels_in_component = 0;
        int count_pixels_in_component = 0;
        int k1 = 0;
        //printf("%d\n", count_pixels_in_component);
        if (x < 1 
            || y < 1 
            || x > (int) component->get_x_size() - 1 
            || y > (int) component->get_y_size() - 1 
            || check_comp_matrix->get_field_value(x,y) == 255 // 255 значит что эту координату мы уже проверяли
            || bin_matrix->get_field_value(x,y) == 0 // 0 значит что эта координата не должна рассматриваться. Она не вошла в срез
            ) 
        return -1; // не ошибка, а выход из рекурсии!!

        else
        {
            check_comp_matrix->set_definite_field_value(x, y, 255); // помечаем что теперь эта координата была рассмотрена
            //count_pixels_in_component = count_pixels_in_component < k + 1 ? k + 1 : count_pixels_in_component;
            if (component->get_field_value(x,y) == 0)
            {
                component->set_definite_field_value(x, y, k);
                count_pixels_in_component += 1;
                //number_wave = k + 1;
            }
            ////count_pixels_in_component += 1;

            ////count_pixels_in_component = count_pixels_in_component < k + 1 ? k + 1 : count_pixels_in_component;
            
            // иначе number_wave не изменяется
            //component->set_definite_field_value(x, y, number_wave);


            k1 = inc(bin_matrix, check_comp_matrix, component, x + 1, y, k + 1);
            if (k1 != -1) count_pixels_in_component += k1;
            //printf("%d\n", count_pixels_in_component);
            k1 = inc(bin_matrix, check_comp_matrix, component, x - 1, y, k + 1);
            if (k1 != -1) count_pixels_in_component += k1;
            //printf("%d\n", count_pixels_in_component);
            k1 = inc(bin_matrix, check_comp_matrix, component, x, y + 1, k + 1);
            if (k1 != -1) count_pixels_in_component += k1;
            //printf("%d\n", count_pixels_in_component);
            k1 = inc(bin_matrix, check_comp_matrix, component, x, y - 1, k + 1);
            if (k1 != -1) count_pixels_in_component += k1;
            //printf("%d\n", count_pixels_in_component);
            k1 = inc(bin_matrix, check_comp_matrix, component, x - 1, y - 1, k + 1);
            if (k1 != -1) count_pixels_in_component += k1;
            //printf("%d\n", count_pixels_in_component);
            k1 = inc(bin_matrix, check_comp_matrix, component, x + 1, y + 1, k + 1);
            if (k1 != -1) count_pixels_in_component += k1;
            //printf("%d\n", count_pixels_in_component);
            k1 = inc(bin_matrix, check_comp_matrix, component, x - 1, y + 1, k + 1);
            if (k1 != -1) count_pixels_in_component += k1;
            //printf("%d\n", count_pixels_in_component);
            k1 = inc(bin_matrix, check_comp_matrix, component, x + 1, y - 1, k + 1);
            if (k1 != -1) count_pixels_in_component += k1;
            //printf("%d\n", count_pixels_in_component);

/*
            count_pixels_in_component = maximum(count_pixels_in_component, inc(bin_matrix, check_comp_matrix, component, x + 1, y, k + 1));
            count_pixels_in_component = maximum(count_pixels_in_component, inc(bin_matrix, check_comp_matrix, component, x - 1, y, k + 1));
            count_pixels_in_component = maximum(count_pixels_in_component, inc(bin_matrix, check_comp_matrix, component, x, y + 1, k + 1));
            count_pixels_in_component = maximum(count_pixels_in_component, inc(bin_matrix, check_comp_matrix, component, x, y - 1, k + 1));
            count_pixels_in_component = maximum(count_pixels_in_component, inc(bin_matrix, check_comp_matrix, component, x - 1, y - 1, k + 1));
            count_pixels_in_component = maximum(count_pixels_in_component, inc(bin_matrix, check_comp_matrix, component, x + 1, y + 1, k + 1));
            count_pixels_in_component = maximum(count_pixels_in_component, inc(bin_matrix, check_comp_matrix, component, x - 1, y + 1, k + 1));
            count_pixels_in_component = maximum(count_pixels_in_component, inc(bin_matrix, check_comp_matrix, component, x + 1, y - 1, k + 1));
*/
        //а ничего что оно ретерн инт а я его не использую. может стоит?
        }
        return count_pixels_in_component;
    }

    
    std::vector<Color> generateColor(int k){ //Генерируем свой цвет для каждого получившегося класстера
        std::vector<Color> colors;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);

        for(int i = 0; i < k; i ++){
            colors.push_back({static_cast<uint8_t>(dis(gen)), 
                              static_cast<uint8_t>(dis(gen)),
                              static_cast<uint8_t>(dis(gen))});
        }

        return colors;
    }


    int Components::kMeans(int k)
    {
        //212-Чернышова-Светлана
        // Алгоритм kMeans
        std::vector<Point> points;
        //---------------------
        int feedback;
        int x_size_of_slice_matrix = int(components_slice_list.matrix_list_first_node->value->get_x_size());
        int y_size_of_slice_matrix = int(components_slice_list.matrix_list_first_node->value->get_y_size());

        Matrix_Node* components_matrix_node = components_slice_list.matrix_list_first_node; //присваивается голова всего списка // указатель типа Node

        Init_field *ptr;
        int comp_value;
        if (components_matrix_node == components_matrix_node->next) // один элемент в списке
        {
            for(int i = 0; i < x_size_of_slice_matrix; ++i)
            {
                for(int j = 0; j < y_size_of_slice_matrix; ++j)
                {
                    ptr = components_matrix_node->value;
                    comp_value = ptr->get_field_value(i, j);
                    if (comp_value != 0)
                    {
                        points.push_back({i, j}); // Вытаскиваем точки из разреза
                    }
                }
            }
        }
        else
        {
            while (components_matrix_node != nullptr)
            {
                for(int i = x_size_of_slice_matrix - 1; i >= 0; --i)
                {
                    for(int j = 0; j < y_size_of_slice_matrix; ++j)
                    {
                        ptr = components_matrix_node->value;
                        comp_value = ptr->get_field_value(i, j);                        
                        if (comp_value != 0)
                        {
                            points.push_back({i, j}); // Вытаскиваем точки из разреза
                        }
                    }
                }
                components_matrix_node = components_matrix_node->next; 
            }
        }
        //--------------------
/*
        for(const auto& comp : components) // для объектов класса Components из списка (будем называть их comp)
        {
            for(int i = 0; i < length; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    if(comp.comp[i][j] == 0) //у меня будет проверка на НЕ равенство нулю!!! 
                    //объект класса Components из списка (comp) обращается к своей матрице comp
                    {
                        points.push_back({i, j}); // Вытаскиваем точки из разреза
                    }
                }
            }
        }
*/

        if (points.empty())
        { 
            // Проверка, смогли ли что-то вытащить 
            return NO_POINTS_FOR_CLUSTERING;
        }

        std::vector<Point> centroids;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_x(0, x_size_of_slice_matrix - 1);
        std::uniform_int_distribution<> dis_y(0, y_size_of_slice_matrix - 1);

        for(int i = 0; i < k; i++){
            centroids.push_back({dis_x(gen), dis_y(gen)}); // Рандомно выбираем k центроид
        }

        bool changed = true;
        std::vector<int> labels(points.size(), -1);

        while(changed){
            changed = false;

            //Шаг 1: назначаем каждую точку ближайшему центроиду
            for(size_t i = 0; i < points.size(); i++)
            {
                double minDist = std::numeric_limits<double>::max();
                int cluster = -1;

                for(int j = 0; j < k; j++)
                { 
                    //Считаем расстояние от точки до каждого класстера, выбираем среди них минимальное
                    double dist = std::pow(points[i].x - centroids[j].x, 2) + std::pow(points[i].y - centroids[j].y, 2);
                    if(dist < minDist)
                    {
                        minDist = dist;
                        cluster = j;
                    }
                }

                if(labels[i] != cluster)
                { 
                    //Проверяем, поменяла хоть одна точка класстер
                    labels[i] = cluster; // Если поменяла, то меняем ее класстер
                    changed = true;
                }
            }

            //Шаг 2: пересчитываем центроиды
            std::vector<Point> newCentroids(k, {0, 0});
            std::vector<int> counts(k, 0);

            for(size_t i = 0; i < points.size(); i++)
            { 
                // Для каждой центроиды складываем все координаты точек, вычисляем количество точек в них
                newCentroids[labels[i]].x += points[i].x;
                newCentroids[labels[i]].y += points[i].y;
                counts[labels[i]]++;
            }

            for(int j = 0; j < k; j++){
                if(counts[j] > 0){
                    newCentroids[j].x /= counts[j];
                    newCentroids[j].y /= counts[j];
                }
            }

            centroids = newCentroids;

        }

        std::vector<Color> clusterColors = generateColor(k); //Генерируем цвета для кластеров

        std::vector<std::vector<Color>> clusterImage(x_size_of_slice_matrix, std::vector<Color>(y_size_of_slice_matrix, {255, 255, 255})); //Заополняем матрицу цветов каждого пикселя
        for(size_t i = 0; i < points.size(); i++){ //Каждой точке -- свой цвет
            int cluster = labels[i];
            clusterImage[points[i].x][points[i].y] = clusterColors[cluster];
        }

        Init_field* pole = new Init_field();
        pole->set_x_size(size_t(x_size_of_slice_matrix));
        pole->set_y_size(size_t(y_size_of_slice_matrix));
        feedback = pole->initialize_with_zeros();
        if (!(feedback == SUCCESS_INIT))
        {
            return FAIL_creating_wave_slice; //возможно не очень верный ретерн
        }

        //Field pole(length, width);
        pole->colors = clusterImage;
        
        pole->make_bmp_clusters("kmeans_clusters.bmp", 2);
        return SUCCESS_kmeans;
    }

//------------------------
    int Components::KMCORES(int k, int p)
    {
        //212-Чернышова-Светлана
        // Алгоритм kMeans with cores

        //Первый этап: стандартный kMeans для поиска кластеров
        // Алгоритм kMeans

        std::vector<Point> points;
        int feedback;
        int x_size_of_slice_matrix = int(components_slice_list.matrix_list_first_node->value->get_x_size());
        int y_size_of_slice_matrix = int(components_slice_list.matrix_list_first_node->value->get_y_size());

        Matrix_Node* components_matrix_node = components_slice_list.matrix_list_first_node; //присваивается голова всего списка // указатель типа Node

        Init_field *ptr;
        int comp_value;
        if (components_matrix_node == components_matrix_node->next) // один элемент в списке
        {
            for(int i = 0; i < x_size_of_slice_matrix; ++i)
            {
                for(int j = 0; j < y_size_of_slice_matrix; ++j)
                {
                    ptr = components_matrix_node->value;
                    comp_value = ptr->get_field_value(i, j);
                    if (comp_value != 0)
                    {
                        points.push_back({i, j}); // Вытаскиваем точки из разреза
                    }
                }
            }
        }
        else
        {
            while (components_matrix_node != nullptr)
            {
                for(int i = x_size_of_slice_matrix - 1; i >= 0; --i)
                {
                    for(int j = 0; j < y_size_of_slice_matrix; ++j)
                    {
                        ptr = components_matrix_node->value;
                        comp_value = ptr->get_field_value(i, j);                        
                        if (comp_value != 0)
                        {
                            points.push_back({i, j}); // Вытаскиваем точки из разреза
                        }
                    }
                }
                components_matrix_node = components_matrix_node->next; 
            }
        }

        if (points.empty())
        { 
            // Проверка, смогли ли что-то вытащить 
            return NO_POINTS_FOR_CLUSTERING;
        }

        std::vector<Point> centroids;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_x(0, x_size_of_slice_matrix - 1);
        std::uniform_int_distribution<> dis_y(0, y_size_of_slice_matrix - 1);

        for(int i = 0; i < k; i++){
            centroids.push_back({dis_x(gen), dis_y(gen)}); // Рандомно выбираем k центроид
        }

        bool changed = true;
        std::vector<int> labels(points.size(), -1);

        while(changed){
            changed = false;

            //Шаг 1: назначаем каждую точку ближайшему центроиду
            for(size_t i = 0; i < points.size(); i++)
            {
                double minDist = std::numeric_limits<double>::max();
                int cluster = -1;

                for(int j = 0; j < k; j++)
                { 
                    //Считаем расстояние от точки до каждого класстера, выбираем среди них минимальное
                    double dist = std::pow(points[i].x - centroids[j].x, 2) + std::pow(points[i].y - centroids[j].y, 2);
                    if(dist < minDist)
                    {
                        minDist = dist;
                        cluster = j;
                    }
                }

                if(labels[i] != cluster)
                { 
                    //Проверяем, поменяла хоть одна точка класстер
                    labels[i] = cluster; // Если поменяла, то меняем ее класстер
                    changed = true;
                }
            }

            //Шаг 2: пересчитываем центроиды
            std::vector<Point> newCentroids(k, {0, 0});
            std::vector<int> counts(k, 0);

            for(size_t i = 0; i < points.size(); i++)
            { 
                // Для каждой центроиды складываем все координаты точек, вычисляем количество точек в них
                newCentroids[labels[i]].x += points[i].x;
                newCentroids[labels[i]].y += points[i].y;
                counts[labels[i]]++;
            }

            for(int j = 0; j < k; j++){
                if(counts[j] > 0){
                    newCentroids[j].x /= counts[j];
                    newCentroids[j].y /= counts[j];
                }
            }

            centroids = newCentroids;
        }
        //================== ok
        

        std::vector<Color> clusterColors = generateColor(k); //Генерируем цвета для кластеров        
        std::vector<std::vector<Color>> clusterImage(x_size_of_slice_matrix, std::vector<Color>(y_size_of_slice_matrix, {255, 255, 255})); //Заополняем матрицу цветов каждого пикселя
        std::vector<std::vector<Point>> clusteredPoints(k);

        for(size_t i = 0; i < points.size(); i++){ //Каждой точке -- свой цвет
            int cluster = labels[i];
            clusterImage[points[i].x][points[i].y] = clusterColors[cluster];
            clusteredPoints[cluster].push_back(points[i]); //Добвавляем каждую точку в свой кластер
        }

        // Второй этап: запуск kMeans для каждого из k кластеров с числом центроид p
        std::vector<std::vector<Point>> subCentroids(k);

        for(int clusterIdx = 0; clusterIdx < k; clusterIdx++){ //Для каждого кластера выполняем алгоритм kMeans
            if(clusteredPoints[clusterIdx].empty()) continue;

            std::vector<Point>& clusterPoints = clusteredPoints[clusterIdx];
            std::vector<Point> subCenters(p);

            //Инициализируем p случайных центроид 
            for(int i = 0; i < p; i++){
                subCenters[i] = clusterPoints[std::uniform_int_distribution<>(0, clusterPoints.size() - 1)(gen)];
            } 

            bool subChanged = true;
            std::vector<int> subLabels(clusterPoints.size(), -1);

            while(subChanged){
                subChanged = false;

                //Назначение точек 
                for(size_t i = 0; i < clusterPoints.size(); i++){
                    double minDist = std::numeric_limits<double>::max();
                    int subCluster = -1; 

                    for(int j = 0; j < p; j++){
                        double dist = std::pow(clusterPoints[i].x - subCenters[j].x, 2) +
                                      std::pow(clusterPoints[i].y - subCenters[j].y, 2);
                        if(dist < minDist){
                            minDist = dist;
                            subCluster = j; 
                        }
                    }

                    if(subLabels[i] != subCluster){
                        subLabels[i] = subCluster;
                        subChanged = true;
                    }
                }

                //Пересчет субцентроид 
                std::vector<Point> newSubCenters(p, {0, 0});
                std::vector<int> subCounts(p, 0);

                for(size_t i = 0; i < clusterPoints.size(); i++){
                    newSubCenters[subLabels[i]].x += clusterPoints[i].x;
                    newSubCenters[subLabels[i]].y += clusterPoints[i].y;
                    subCounts[subLabels[i]]++;
                }

                for(int j = 0; j < p; j++){
                    if(subCounts[j] != 0){
                        newSubCenters[j].x /= subCounts[j];
                        newSubCenters[j].y /= subCounts[j];
                    }
                }

                subCenters = newSubCenters;
            }
            subCentroids[clusterIdx] = subCenters;

            //Визуализация субцентроид 
            for(const auto& subCenter: subCenters){
                clusterImage[subCenter.x][subCenter.y] = {0, 0, 0}; //Цвет для центров тяжести
            }
        }


        Init_field* pole = new Init_field();
        pole->set_x_size(size_t(x_size_of_slice_matrix));
        pole->set_y_size(size_t(y_size_of_slice_matrix));
        feedback = pole->initialize_with_zeros();
        if (!(feedback == SUCCESS_INIT))
        {
            return FAIL_creating_wave_slice;
        }

        //Field pole(length, width);
        pole->colors = clusterImage;
        
        pole->make_bmp_clusters("kmcores_clusters.bmp", 2);

/*
        Field pole(length, width);  
        pole.colors = clusterImage;
        pole.bmp_write("clusters.bmp", 2);
*/    
        return SUCCESS_KMCORES;
    }
//------------------------------------

/*
    Components::~Components()
    {

    }
*/

//212-Chernyshova-Svetlana
    //class Gauss

// Constructor :
    Gauss::Gauss(int h, int x, int y, int s_x, int s_y)
    {
        hight = h;
        mu_x = x;
        mu_y = y;
        sigma_x = s_x;
        sigma_y = s_y;
    }

//место для сетов - вроде не нужны
//место для гетов
    int Gauss::get_hight() const {return hight;}
    int Gauss::get_mu_x() const {return mu_x;}
    int Gauss::get_mu_y() const {return mu_y;}
    int Gauss::get_sigma_x() const {return sigma_x;}
    int Gauss::get_sigma_y() const {return sigma_y;}


/*
        double make_gauss(double x, double y, double h, double mu_x, double mu_y, double sigma_x, double sigma_y)
        {
            double f;
            f = 1/(2*pi*sigma_x*sigma_y)*pow(e, - (x - mu_x) * (x - mu_x) / (2 * sigma_x * sigma_x) - (y - mu_y) * (y - mu_y) / (2 * sigma_y * sigma_y));
            return f;
        }
*/

//212-Chernyshova-Svetlana
    //struct Node
//constructor:
        // тут присвоение Гаусса выходит...
    Node::Node(Gauss val) : next(nullptr)
    {
        value = val;
    } // напоминание после : идут значения по умолчанию с помощью конструкторов стандартных типов

    Matrix_Node::Matrix_Node(Gauss_program::Init_field* val) : next(nullptr)
    {
        value = val; //тут это присвоение указателей
        val = nullptr;
    } 

/*
    Matrix_Node::~Matrix_Node()  // указатель занулится автоматически, а удалять значение по нему нам и не надо, там другой узел уже, а гаусс будет уничтожен своим деструктором
    {

    }
*/

    //struct List
//constructor:
    List::List() : first_node(nullptr), last_node(nullptr) {}

    Matrix_List::Matrix_List() : matrix_list_first_node(nullptr), matrix_list_last_node(nullptr) {}

//destructor:        
    bool List::list_is_empty()
    {
        return first_node == nullptr;
    }
       
    bool Matrix_List::matrix_list_is_empty()
    {
        return matrix_list_first_node == nullptr;
    }

    List::~List()
    {
        while (!list_is_empty())
        {
            remove();
        }
        //first_node = nullptr;
        //last_node = nullptr;
    }

    Matrix_List::~Matrix_List()
    {
        while (!matrix_list_is_empty())
        {
            matrix_list_remove();
        }
    }

/*
    Matrix_Node::~Matrix_Node()
    {
        delete value;
        value = nullptr;
        next = nullptr;
    }
*/

    void List::push_back (Gauss val)
    //212-Chernyshova-Svetlana
    {
        Node* new_node = new Node(val); //создается пара объект гаусс и пустой указатель
        if (list_is_empty())
        {
            first_node = new_node;
            last_node = new_node;
        }
        //если список не пуст
        last_node->next = new_node; // указателю последнего созданного узла на следующий элемент списка присваиваем указатель на новый узел
        last_node = new_node; // меняем адрес последнего узла на новодобавленный ?а куда мы дели прошлый узел?
        //delete new_node; //не лишнее ли это тут? - лишнее тк я хочу этим еще пользоваться после завершения функции
    }

    void Matrix_List::matrix_list_push_back (Gauss_program::Init_field* val)
    //212-Chernyshova-Svetlana
    {
        Matrix_Node* matrix_list_new_node = new Matrix_Node(val); //создается пара объект гаусс и пустой указатель
        if (matrix_list_is_empty())
        {
            matrix_list_first_node = matrix_list_new_node;
            matrix_list_last_node = matrix_list_new_node;
        }
        //если список не пуст
        matrix_list_last_node->next = matrix_list_new_node; // указателю последнего созданного узла на следующий элемент списка присваиваем указатель на новый узел
        matrix_list_last_node = matrix_list_new_node; // меняем адрес последнего узла на новодобавленный ?а куда мы дели прошлый узел?
        //delete matrix_list_new_node; //не лишнее ли это тут? - лишнее тк я хочу этим еще пользоваться после завершения функции
    }

    void List::remove ()
    //212-Chernyshova-Svetlana
    {
        if (!list_is_empty()) // if the list is't empty
        {
            //std::cout<<"First_node : "<<first_node<<"\n";
            ///if (first_node->next != nullptr) //почему он ругается на обращение к указателю???
            ///{
            if (first_node->next == first_node)
            {
                delete first_node;
                first_node = nullptr;
            }
            else
            {
                Node * tmp = first_node;
                ///std::cout<<"Tmp = first_node : "<<tmp<<"\n";
                ///std::cout<<"First_node : "<<first_node<<"\n";
                first_node = tmp->next;
                ///std::cout<<"First_node = tmp->next : "<<first_node<<"\n";
                ///std::cout<<"Tmp : "<<tmp<<"\n";
                //delete &(tmp->value);
                delete tmp;
                ///std::cout<<"Tmp after delete : "<<tmp<<"\n";
                ///std::cout<<"First_node : "<<first_node<<"\n";
            }
            ///}
            ///else
            ///{
            ///    Node * tmp = first_node;
            ///    delete tmp;
            ///}
            // first_node = first_node -> next; // а так можно??
            // иначе могли бы возбудить исключение
        }
    }

    
    void Matrix_List::matrix_list_remove ()
    //212-Chernyshova-Svetlana
    {
        if (!matrix_list_is_empty()) // if the list is't empty
        {
            ////std::cout<<"matrix_list_first_node : "<<matrix_list_first_node<<"\n";
            ///if (first_node->next != nullptr) //почему он ругается на обращение к указателю???
            ///{
            if (matrix_list_first_node->next == matrix_list_first_node)
            {
                delete matrix_list_first_node;
                matrix_list_first_node = nullptr;
            }
            else
            {
                Matrix_Node * tmp = matrix_list_first_node;
                //std::cout<<"Tmp = matrix_list_first_node : "<<tmp<<"\n";
                //std::cout<<"matrix_list_first_node : "<<matrix_list_first_node<<"\n";
                ///delete first_node;
                matrix_list_first_node = tmp->next;
                //std::cout<<"matrix_list_first_node = tmp->next : "<<matrix_list_first_node<<"\n";
                //std::cout<<"Tmp : "<<tmp<<"\n";
                /////////////delete &(tmp->value);
                delete tmp;
                //std::cout<<"Tmp after delete : "<<tmp<<"\n";
                //std::cout<<"matrix_list_first_node : "<<matrix_list_first_node<<"\n";
                tmp = nullptr;
            }
        }
    }



    void List::generate_field (Init_field* field)
    //212-Chernyshova-Svetlana
    //gen realization
    {
        double f;
        int x = 0, y = 0;
        int value;
        int check_value;
        //int tmp_value;
        int hight, mu_x, mu_y, sigma_x, sigma_y;
        int h;
        int s_x, s_y;


        Node* node = first_node; //присваивается голова всего списка // указатель типа Node
        // в строчке выше сработало через точку, тк есть объект типа List
        while (node != nullptr)
        {
            x = y = 0;
            Gauss g = node->value;
            
            hight = g.get_hight();
            mu_x = g.get_mu_x();
            mu_y = g.get_mu_y();
            sigma_x = g.get_sigma_x();
            sigma_y = g.get_sigma_y();
            h = int(modul(127. - hight));

            //int s_x = h * sigma_x, s_y = h * sigma_y;
            s_x = sigma_x, s_y = sigma_y;
            //int s_x = hight * sigma_x, s_y = hight * sigma_y;
            
            ///printf("7\n");
            ///printf("MU_X: %d\nSigma_x: %d\nSigma*h: %d\n", mu_x, sigma_x, s_x);
            ///printf("MAX: %d\n", maximum(0, mu_x - 3*s_x));
            /////size_t si = field->get_x_size();
            ///printf("size: %zd\n", si);
            /////int w = int(si);
            ///printf("w: %d\n",w);
            ///printf("MIN: %d\n", minimum(int(field->get_x_size()), mu_x + 3*s_x));

            for (x = 0; x < int(field->get_x_size()); x++)
            //for (x = maximum(0, mu_x - 2.5*s_x); x < minimum(int(field->get_x_size()), mu_x + 2.5*s_x); x++)
            {
                for (y = 0; y < int(field->get_y_size()); y++)
                //for (y = maximum(0, mu_y - 2.5*s_y); y < minimum(int(field->get_y_size()), mu_y + 2.5*s_y); y++)
                {


                    //printf("7787\n");
                    ///printf("x : %d; y : %d\n",x,y);
                    ///printf("d : %d\n" , hight - 127);
                    ///printf("s_x : %d\ns_y : %d\nmu_x : %d\nmu_y : %d\npow(e,1) : %lf\n", s_x, s_y, mu_x, mu_y,pow(e,1));
                    //f = (h - 127.) * (1. / (2 * pi * s_x*h * s_y*h) * pow(e, - (x - mu_x) * (x - mu_x) / (2 * s_x*h * s_x*h) - (y - mu_y) * (y - mu_y) / (2 * s_y*h * s_y*h))) + 127;
                    //f = int((hight - 127.)/10.) * ((1. / (2 * pi * s_x * s_y) * pow(e, - (x - mu_x) * (x - mu_x) / (2 * s_x * s_x) - (y - mu_y) * (y - mu_y) / (2 * s_y * s_y))) + 127);
                    //f = 1. / (2 * pi * s_x * s_y) * pow(e, - (double(x)/h - mu_x) * (double(x)/h - mu_x) / (2 * s_x * s_x) - (double(y)/h - mu_y) * (double(y)/h - mu_y) / (2 * s_y * s_y)) + 127;
                    //f = 1.5*255*(double(hight) - 127) / (2 * pi * s_x * s_y) * pow(e, - (double(x) - mu_x) * (double(x) - mu_x) / (2 * s_x * s_x) - (double(y) - mu_y) * (double(y) - mu_y) / (2 * s_y * s_y)) + 127;
                    f = 900.* h * ((double(hight) - 127) / h) / (2. * pi * s_x * s_y) * pow(e, - (double(x) - mu_x) * (double(x) - mu_x) / (2 * s_x * s_x) - (double(y) - mu_y) * (double(y) - mu_y) / (2 * s_y * s_y)) + 127;
                    //f = 1. * h * ((double(hight) - 127) / h) / (2. * pi * s_x * s_y) * pow(e, - (double(x) - mu_x) * (double(x) - mu_x) / (2 * s_x * s_x) - (double(y) - mu_y) * (double(y) - mu_y) / (2 * s_y * s_y)) + 127;

                    ///printf("f : %lf\n", f);
                    value = ((f - floor(f)) <= 0.5) ? (floor(f)) : (floor(f) + 1);
                    //tmp_value = ((f - floor(f)) <= 0.5) ? (floor(f)) : (floor(f) + 1);
/*
                    if ((tmp_value >= 0) && (tmp_value <= 255))
                    {
                        value = tmp_value;
                    }
                    else if (tmp_value < 0)
                    {
                        value = 0;
                    }
                    else
                    {
                        value = 255;
                    }
*/
                    ///printf("Set_field_value\n");
                    field->set_field_value(x, y, value);
                    //if (field->get_field_value(x, y) != 127)
                    //if (field->get_field_value(x, y) < 120 || field->get_field_value(x, y) > 134)
                    //{
                    //    field->weight_matrix[x][y] += 1;
                    //}
                }
                //printf("25\n");
            }
            node = node->next; // присваиваем адрес следующего узла, хранящийся в next-е
            //printf("555\n");
            
            //field_generation = 1; //точно ли могу вынести эту штуку отсюда
        }
        for (x = 0; x < int(field->get_x_size()); x++)
        {
            for (y = 0; y < int(field->get_y_size()); y++)
            {
                check_value = field->get_field_value(x,y);
                if (check_value < 0)
                {
                    field->set_definite_field_value(x,y,0);
                }
                else if (check_value > 255)
                {
                    field->set_definite_field_value(x,y,255);
                }
                ////printf("%d ", field->get_field_value(x,y));
            }
            ////printf("\n");
        }
    }

    //class Log_control_file

    void Log_control_file::enable_control_logging(const std::string& filename)
    { 
        // Функция для включения логирования сервера
        control_logging_enabled = true;
        Log_Control_filename = filename;
        if(control_log.is_open())
        {
            control_log.close();
        }
        control_log.open(Log_Control_filename, std::ios::app);
    }

    void Log_control_file::disable_control_logging()
    { 
        // Функция для отключения логирования сервера
        control_logging_enabled = false;
        if(control_log.is_open())
        {
            control_log.close();
        }
    }

    std::string Log_control_file::current_control_time() 
    {
        // Как будет отображаться время в логах
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(now, "%d.%m.%Y %H:%M:%S");
        return oss.str();
    }
    
    void Log_control_file::logs_of_control(const std::string& message) 
    { 
        // Функция-шаблон лога
        if (control_logging_enabled && control_log.is_open()) 
        {
            control_log << current_control_time() << " : " << message << std::endl;
        }
    }


    //class Log_interface_file


    void Log_interface_file::enable_interface_logging(const std::string& filename)
    { 
        // Функция для включения логирования сервера
        interface_logging_enabled = true;
        Log_Interface_filename = filename;
        if(interface_log.is_open())
        {
            interface_log.close();
        }
        interface_log.open(Log_Interface_filename, std::ios::app);
    }

    void Log_interface_file::disable_interface_logging()
    { 
        // Функция для отключения логирования сервера
        interface_logging_enabled = false;
        if(interface_log.is_open())
        {
            interface_log.close();
        }
    }

    std::string Log_interface_file::current_interface_time() 
    {
        // Как будет отображаться время в логах
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(now, "%d.%m.%Y %H:%M:%S");
        return oss.str();
    }
    
    void Log_interface_file::log(const std::string& message) 
    { 
        // Функция-шаблон лога
        if (interface_logging_enabled && interface_log.is_open()) 
        {
            interface_log << current_interface_time() << " : " << message << std::endl;
        }
    }

    //void Log_interface_file::feedback_printing(Gauss_program::Log_interface_file interface_logging, int feedback)
    void Log_interface_file::feedback_printing(int feedback)
    //212-Chernyshova-Svetlana
    //part of control's logs
    {
        // возможно предупреждение о повторном открытии файла. тогда все fprintf заменяются на функцию log
        //FILE* local_interface_log = fopen(interface_log_filename,"a");

        //    interface_logging.log("");


        switch (feedback)
        {
            case SUCCESS_INIT:
                //interface_logging.log("Field successfully initialized!\n");
                log("REPORT : Field successfully initialized!\n");
                break;
            case FAIL_INIT:
                //interface_logging.log("Can't initialize a field.\n");
                log("REPORT : Can't initialize a field.\n");
                break;
            case SECOND_FIELD_INITIALIZATION:
                //interface_logging.log("The field already exsists, there is no opportunity to make one more. The command skipped.\n");
                log("REPORT : The field already exsists, there is no opportunity to make one more. The command skipped.\n");
                break;
            case FIELD_INIT_TRY_AFTER_BMP_READ:
                //interface_logging.log("The field already exsists, it was read from a bmp file, there is no opportunity to make one more with initialization. The command skipped.\n");
                log("REPORT : The field already exsists, it was read from a bmp file, there is no opportunity to make one more with initialization. The command skipped.\n");
                break;
            case GAUSS_ADD_TRY_AFTER_BMP_READ:
                //interface_logging.log("The field already exsists, it was read from a bmp file, there is no opportunity to make if with gausses, so we are not going to remember them. The command skipped.\n");
                log("REPORT : The field already exsists, it was read from a bmp file, there is no opportunity to make if with gausses, so we are not going to remember them. The command skipped.\n");
                break;
            case NO_bmp_reading_after_gausses:
                //interface_logging.log("The gausses has already been added. Not able to use bmp reading for field initialization. The command skipped.\n");
                log("REPORT : The gausses has already been added. Not able to use bmp reading for field initialization. The command skipped.\n");
                break;
            case ERROR_UNINITIALIZED_FIELD:
                //interface_logging.log("Generation, gnuplot and bmp can't be complete without a field. The field has't been initialized. The command skipped.\n");
                log("REPORT : Generation, gnuplot and bmp can't be complete without a field. The field has't been initialized. The command skipped.\n");
                break;
            //case ERROR_ALREADY_INITIALIZED_FIELD:
            //    printf("The field has already been initialized with Gausses. Error: second initialization. The command skipped.\n");
            //    break;
            case USAGE_wrong_command_input:
                //interface_logging.log("Wrong input of the command.\n");
                log("REPORT : Wrong input of the command.\n");
                break;
            case SUCCESS_GEN:
                //interface_logging.log("The field generation is complete.\n");
                log("REPORT : The field generation is complete.\n");
                break;
            case SUCCESS_GNU:
                //interface_logging.log("Gnuplot file was successfully created.\n");
                log("REPORT : Gnuplot file was successfully created.\n");
                break;
            case SUCCESS_BMP_WRITE:
                //interface_logging.log("BMP file was successfully created.\n");
                log("REPORT : BMP file was successfully created.\n");
                break;
            case SUCCESS_BMP_READ:
                //interface_logging.log("BMP file was successfully read.\n");
                log("REPORT : BMP file was successfully read.\n");
                break;
            case BMP_FILE_FOR_READING_NOT_FOUND:
                //interface_logging.log("File for reading BMP is not found.\n");
                log("REPORT : File for reading BMP is not found.\n");
                break;
            case FAIL_BMP_opening:
                //interface_logging.log("File for writing BMP is not found.\n");
                log("REPORT : File for writing BMP is not found.\n");
                break;
            case GNU_fail_not_generated_field:
                //interface_logging.log("Can't make gnuplot because there are no gausses on the field.\n");
                log("REPORT : Can't make gnuplot because there are no gausses on the field. The command skipped.\n");
                break;
            case SUCCESS_GSS:
                //interface_logging.log("Gauss was successfully created.\n");
                log("REPORT : Gauss was successfully created.\n");
                break;
            case BIN_SUCCESS:
                //interface_logging.log("The slice matrix is successfully created and added to the list.\n");
                log("REPORT : The slice matrix is successfully created and added to the list.\n");
                break;
            case FAIL_creating_wave_slice:
                //interface_logging.log("Not able to run WAVE : failed to create wave slice.\n");
                log("REPORT : Not able to run WAVE : failed to create wave slice.\n");
                break;
            case FAIL_bin_matrix_init:
                //interface_logging.log("The slice matrix wasn't initialized. Sth went wrong.\n");
                log("REPORT : The slice matrix wasn't initialized. Sth went wrong.\n");
                break;
            case IMPOSSIBLE_CUT_VALUE:
                //interface_logging.log("This value can't be used for cutting.\n");
                log("REPORT : This value can't be used for cutting.\n");
                break;
            case NO_FIELD_FOR_CUTTING:
                //interface_logging.log("There is no any field yet, so cutting is impossible. The command skipped.\n");
                log("REPORT : There is no any field yet, so cutting is impossible. The command skipped.\n");
                break;
            case NO_FIELD_FOR_WAVE:
                //interface_logging.log("There is no any field yet, so wave command is impossible. The command skipped.\n");
                log("REPORT : There is no any field yet, so wave command is impossible. The command skipped.\n");
                break;
            case NO_FIELD_FOR_KMEANS:
                //interface_logging.log("There is no any field yet, so kmeans command is impossible. The command skipped.\n");
                log("REPORT : There is no any field yet, so kmeans command is impossible. The command skipped.\n");
                break;
            case NO_FIELD_FOR_KMCORES:
                log("REPORT : There is no any field yet, so kmCores command is impossible. The command skipped.\n");
                break;
            case WAVE_Error_writing_bmp:
                //interface_logging.log("In wave : error writing BMP!\n");
                log("REPORT : In wave : error writing BMP!\n");
                break;
            case SUCCESS_WAVE:
                //interface_logging.log("WAVE successfully completed.\n");
                log("REPORT : WAVE successfully completed.\n");
                break;
            case NO_POINTS_FOR_CLUSTERING:
                //interface_logging.log("No points available for clustering.\n");
                log("REPORT : No points available for clustering.\n");
                break;
            case SUCCESS_kmeans:
                //interface_logging.log("Kmeans successully completed.\n");
                log("REPORT : Kmeans successully completed.\n");
                break;
            case SUCCESS_KMCORES:
                log("REPORT : KMCORES successully completed.\n");
                break;
            default:
                //interface_logging.log("Something happend, but I do not know what...\n");
                log("REPORT : Something happend, but I do not know what...\n");
                break;
        }
        //fclose(local_interface_log);
    }

/*
    void Log_interface_file::feedback_printing(char* interface_log_filename, int feedback)
    //212-Chernyshova-Svetlana
    //part of control's logs
    {
        // возможно предупреждение о повторном открытии файла. тогда все fprintf заменяются на функцию log
        FILE* local_interface_log = fopen(interface_log_filename,"a");
        switch (feedback)
        {
            case SUCCESS_INIT:
                fprintf(local_interface_log, "Field successfully initialized!\n");
                break;
            case FAIL_INIT:
                fprintf(local_interface_log, "Can't initialize a field.\n");
                break;
            case SECOND_FIELD_INITIALIZATION:
                fprintf(local_interface_log, "The field already exsists, there is no opportunity to make one more. The command skipped.\n");
                break;
            case FIELD_INIT_TRY_AFTER_BMP_READ:
                fprintf(local_interface_log, "The field already exsists, it was read from a bmp file, there is no opportunity to make one more with initialization. The command skipped.\n");
                break;
            case GAUSS_ADD_TRY_AFTER_BMP_READ:
                fprintf(local_interface_log, "The field already exsists, it was read from a bmp file, there is no opportunity to make if with gausses, so we are not going to remember them. The command skipped.\n");
                break;
            case NO_bmp_reading_after_gausses:
                fprintf(local_interface_log, "The gausses has already been added. Not able to use bmp reading for field initialization. The command skipped.\n");
                break;
            case ERROR_UNINITIALIZED_FIELD:
                fprintf(local_interface_log, "Generation, gnuplot and bmp can't be complete without a field. The field has't been initialized. The command skipped.\n");
                break;
            //case ERROR_ALREADY_INITIALIZED_FIELD:
            //    printf("The field has already been initialized with Gausses. Error: second initialization. The command skipped.\n");
            //    break;
            case USAGE_wrong_command_input:
                fprintf(local_interface_log, "Wrong input of the command.\n");
                break;
            case SUCCESS_GEN:
                fprintf(local_interface_log, "The field generation is complete.\n");
                break;
            case SUCCESS_GNU:
                fprintf(local_interface_log, "Gnuplot file was successfully created.\n");
                break;
            case SUCCESS_BMP_WRITE:
                fprintf(local_interface_log, "BMP file was successfully created.\n");
                break;
            case SUCCESS_BMP_READ:
                fprintf(local_interface_log, "BMP file was successfully read.\n");
                break;
            case BMP_FILE_FOR_READING_NOT_FOUND:
                fprintf(local_interface_log, "File for reading BMP is not found.\n");
                break;
            case FAIL_BMP_opening:
                fprintf(local_interface_log, "File for writing BMP is not found.\n");
                break;
            case GNU_fail_not_generated_field:
                fprintf(local_interface_log, "Can't make gnuplot because there are no gausses on the field.\n");
                break;
            case SUCCESS_GSS:
                fprintf(local_interface_log, "Gauss was successfully created.\n");
                break;
            case BIN_SUCCESS:
                fprintf(local_interface_log, "The slice matrix is successfully created and added to the list.\n");
                break;
            case FAIL_creating_wave_slice:
                fprintf(local_interface_log, "Not able to run WAVE : failed to create wave slice.\n");
                break;
            case FAIL_bin_matrix_init:
                fprintf(local_interface_log, "The slice matrix wasn't initialized. Sth went wrong.\n");
                break;
            case IMPOSSIBLE_CUT_VALUE:
                fprintf(local_interface_log, "This value can't be used for cutting.\n");
                break;
            case NO_FIELD_FOR_CUTTING:
                fprintf(local_interface_log, "There is no any field yet, so cutting is impossible. The command skipped.\n");
                break;
            case NO_FIELD_FOR_WAVE:
                fprintf(local_interface_log, "There is no any field yet, so wave command is impossible. The command skipped.\n");
                break;
            case NO_FIELD_FOR_KMEANS:
                fprintf(local_interface_log, "There is no any field yet, so kmeans command is impossible. The command skipped.\n");
                break;
            case WAVE_Error_writing_bmp:
                fprintf(local_interface_log, "In wave : error writing BMP!\n");
                break;
            case SUCCESS_WAVE:
                fprintf(local_interface_log, "WAVE successfully completed.\n");
                break;
            case NO_POINTS_FOR_CLUSTERING:
                fprintf(local_interface_log, "No points available for clustering.\n");
                break;
            case SUCCESS_kmeans:
                fprintf(local_interface_log, "Kmeans successully completed.\n");
                break;
            default:
                fprintf(local_interface_log, "Something happend, but I do not know what...\n");
                break;
        }
        fclose(local_interface_log);
    }
*/

/*
    class log_file
    {
        private:
        char NO_PROBLEM[40] = "The command is successfully accepted";
        char PROBLEM[30] = "Something went wrong";
        char log_file_name[30] = "log_file.txt";
        char class1[30] = "Started using class Interface";
        char class2[30] = "Started using class Control";
        char class3[30] = "Started using class Gauss";

        public:
        void Printing_success_to_the_log_file(int using_class)
        {
            switch (using_class)
            {
                case 1:

                case 2:

                case 3:

            }
        }

        void Printing_success_to_the_log_file(int error)
        {
            if (error == 0) 
            {
                fprintf(log_file_name, "%s", NO_PROBLEM);
            }
            else
            {
                fprintf(log_file_name, "%s", PROBLEM);
            }
        }
    };
*/

//212-Chernyshova-Svetlana
//some functions
int maximum (int x, int y) { return x > y ? x : y; }
int minimum (int x, int y) { return x > y ? y : x; }
double modul (double x) { return x > 0 ? x : -x; }
