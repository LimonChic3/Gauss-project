#ifndef group_212_Chernyshova_Svetlana_Gauss_functions
#define group_212_Chernyshova_Svetlana_Gauss_functions

#include <ctime> 
#include <fstream> 
#include <iostream> 
#include <sstream>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <random>
#include <iomanip>



#include <cstdint> //
#include <chrono>


#define pi 3.14159
#define e 2.71828
#define N 256
#define MAX_AMOUNT 1234
#define LEN_SMALL 10
//enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL }; 
using namespace std;
namespace Gauss_program
{
    struct Point
    { 
        // структура для хранения координат точки
        int x,y;
    };

    struct Color
    { 
        // структура для хранения rgb цвета
        uint8_t r, g, b;
    };

    class Init_field
    {
        private:
        size_t x_size = 0, y_size = 0;
        int *field_arr = nullptr;
        int initialization_worked = 0; //flag for destructor
        //std::vector<std::vector<Color>> colors;
        public:
        std::vector<std::vector<double>> weight_matrix; // Для НОРМАЛЬНОЙ нормировки при накладывании!
        std::vector<std::vector<Color>> colors;
        const Init_field& operator= (const Init_field& g) = delete; 
    // constructors:
    // Copy constructor
        Init_field(const Init_field& copied) = delete; // закомменчено из-за жаловы компилятора что в Control-e не получается с field работать
    /*
        {
            // сейчас напишу плохой конструктор копирования для которого надо обязательно занулять *field перед выходом из функции
            x_size = copied.x_size;
            y_size = copied.y_size;
            field_arr = copied.field_arr; // very-very bad
        }
    */
    // Move constructor 
        Init_field(Init_field && moved) = delete;
    // Constructor
        Init_field() 
        {
            x_size = 0;
            y_size = 0;
        }
        //Init_field(size_t x_size, size_t y_size);
    // functions
        int initialize();
        int initialize_with_zeros();
        size_t get_x_size() const;
        size_t get_y_size() const;
        void set_x_size(size_t n_x);
        void set_y_size(size_t n_y);
        int get_field_value(int x, int y);
        void set_definite_field_value(int x, int y, int set_definite_value);
        void set_field_value(int x, int y, int set_value);
        int gnuplot(char * GNU_data_file_name, char * GNU_file_name) const;
        int make_bmp(const char * BMP_file_name);
        int make_bmp_clusters(const char * filename, int k);
        int read_bmp(Gauss_program::Init_field* field, char* BMP_file_name);
    // Destructor
        ~Init_field();
    };
    
    class Gauss
    {

        private:
    //    double x = 1, y = 1; //переменные будут уже в generate
        int hight = 1, mu_x = 1, mu_y = 1;
        int sigma_x = 1, sigma_y = 1;

        public:
    // Constructor:
        Gauss() = default;
        Gauss(int h, int x, int y, int s_x, int s_y);
    //место для оператора присваивания
        const Gauss& operator= (const Gauss& g)
        {
            hight = g.hight;
            mu_x = g.mu_x; mu_y = g.mu_y; // у оператора класса есть доступ к приватным полям класса 
            sigma_x = g.sigma_x; sigma_y = g.sigma_y;
            return *this;
        }
    //место для конструктора копирования // я вот сомневаюсь что он нужен был
        Gauss (const Gauss& g) = default;
    /*
            {
                hight = g.hight;
                mu_x = g.mu_x;
                mu_y = g.mu_y;
                sigma_x = g.sigma_x;
                sigma_y = g.sigma_y;            
            }
    */
    //место для деструктора
        ~Gauss() = default;

        int get_hight() const;
        int get_mu_x() const;
        int get_mu_y() const;
        int get_sigma_x() const;
        int get_sigma_y() const;
    };

    struct Node 
    {
        Gauss value;
        Node* next; //будет указывать на след элемент типа Node   
//constructor:
        Node(Gauss val);
//destructor:
        ~Node() = default; // указатель занулится автоматически, а удалять значение по нему нам и не надо, там другой узел уже, а гаусс будет уничтожен своим деструктором

    };

    class List
    {
        public:
        Node* first_node;
        Node* last_node;
        List();
        ~List();
        bool list_is_empty();
        void push_back (Gauss val);
        void remove();
        void generate_field(Gauss_program::Init_field* field);
    };

//    class Matrix_Node 
    struct Matrix_Node // Доступ будет только к указателю. Само поле защищено
    {
        //private:
        Init_field *value = nullptr;
        Matrix_Node* next = nullptr; //будет указывать на след элемент типа Node
        //public:
//constructor:
        Matrix_Node(Gauss_program::Init_field* val);
//destructor:
        //~Matrix_Node();// = default; // указатель занулится автоматически, а удалять значение по нему нам и не надо, там другой узел уже, а гаусс будет уничтожен своим деструктором
        ~Matrix_Node() = default; // указатель занулится автоматически, а удалять значение по нему нам и не надо, там другой узел уже, а гаусс будет уничтожен своим деструктором

    };

    class Matrix_List
    {
        public:
        Matrix_Node* matrix_list_first_node;
        Matrix_Node* matrix_list_last_node;
        Matrix_List();
        ~Matrix_List();
        //private:
        bool matrix_list_is_empty();
        void matrix_list_push_back (Gauss_program::Init_field* val);
        void matrix_list_remove();
//        void generate_field(Gauss_program::Init_field* field);
    };

    class Components
    {
        private:
        Matrix_List bin_slice_list; //ему надо свой лист писать
        Matrix_List components_slice_list; 
        //Init_field* matrix_without_small_components = nullptr;
        //Init_field* matrix_without_small_components = new Init_field();
        int amount_of_bin_slices = 0;
        int amount_of_components = 0;
        int count_pixels_in_component = 0;
        public:
        Components() = default;
        ~Components() = default;
        int bin(Gauss_program::Init_field* field, int cut_value);
        int wave_start(int component_size);
        int wave(Gauss_program::Init_field* bin_matrix, int component_size, int bin_slice_number);
        int inc(Gauss_program::Init_field* bin_matrix, Gauss_program::Init_field* check_comp_matrix, Gauss_program::Init_field* component, int x, int y, int k);
        int kMeans(int k);
        int KMCORES(int k, int p);
    };


    class Log_control_file
    {
        public:
        std::ofstream control_log; //FILE*
        bool control_logging_enabled = false;
        std::string Log_Control_filename = "control_log_ex_in_prog.txt";

        void enable_control_logging(const std::string& filename);
        void disable_control_logging();
        std::string current_control_time();

        //void feedback_printing(char* interface_log_file, int feedback);
        //void feedback_printing(Gauss_program::Log_interface_file interface_logging, int feedback);
        //void feedback_printing(int feedback);
        void logs_of_control(const std::string& message);

        Log_control_file() = default;
        ~Log_control_file() 
        {
            if (control_log.is_open()) 
            {
                control_log.close();
            }
        }
    };


    class Control
    {
        private:
        static int field_initialization;
        static int field_generation;
        static int bmp_reading_initialization;
        int amount_of_gauss = 0;
        List l;
        Components comp_obj;
        /////////////////////Log_control_file log_control_file_obj;
        
        //Log_Control_file control_logging;
        
        public:
        Log_control_file control_logging;

    // Constructor:
        Control() = default; // ?норм?
    // Destructor:
        ~Control() = default;  // разве что не надо ли удалить статик переменную отдельно
            // чего делать с объектами в деструкторе????
            // деструктор для list запустится автоматически, тк это объект а не ссылка на него, аналогично с переменными
        int get_amount_of_gauss() const {return amount_of_gauss;}

        int get_amount_of_gauss();
        int init_f (size_t n_x, size_t n_y, Gauss_program::Init_field * field);
        int generate_field_with_gausses(Gauss_program::Init_field* field);
        int make_gnuplot_file(Gauss_program::Init_field* field, char * GNU_data_file_name, char * GNU_file_name);
        int make_gauss(int h, int x, int y, int s_x, int s_y);
        int make_bmp_file(Gauss_program::Init_field* field, const char * BMP_created_file_name);
        int read_bmp_file(Gauss_program::Init_field* field, char * BMP_created_file_name);
        int bin_cutting(Gauss_program::Init_field* field, int cut_value);
        int wave_command(int component_size);
        int kMeans_command(int k);
        int KMCORES_command(int k, int p);

    };

    class Log_interface_file
    {
        public:
        std::ofstream interface_log; //FILE*
        bool interface_logging_enabled = false;
        std::string Log_Interface_filename = "interface_log_ex_in_prog.txt";

        void enable_interface_logging(const std::string& filename);
        void disable_interface_logging();
        std::string current_interface_time();

        //void feedback_printing(char* interface_log_file, int feedback);
        //void feedback_printing(Gauss_program::Log_interface_file interface_logging, int feedback);
        void feedback_printing(int feedback);
        void log(const std::string& message);

        Log_interface_file() = default;
        ~Log_interface_file() 
        {
            if (interface_log.is_open()) 
            {
                interface_log.close();
            }
        }
    };

    class Interface
    {

        private:
        //int message = -1;
        Control control_obj;
        Init_field *field = new Init_field();

        public:
        Log_interface_file interface_logging;


// значения по умолчанию
        std::string interface_log_filename;

        std::string interface_Batch_filename;
        int interface_Field_DEF_x_size, interface_Field_DEF_y_size;
        int interface_Gauss_DEF_hight, interface_Gauss_DEF_mu_x, interface_Gauss_DEF_mu_y;
        int interface_Gauss_DEF_sigma_x, interface_Gauss_DEF_sigma_y;

        const char *interface_BMP_creation_filename;
        char *interface_BMP_reading_filename;
        char *interface_Gnuplot_data_filename;
        char *interface_Gnuplot_commands_filename;
        int interface_BIN_hight;
        int interface_WAVE_min_component_size;
        int interface_KMEANS_amount_of_dots;
        int interface_KMCORE_amount_of_cores, interface_KMCORE_amount_of_dots_in_cores;


        Interface() = default;
        ~Interface();
        int command_receiving_place(FILE ** input_stream_name);
        int understand_command(char * get_command_from);
        std::string trim(const std::string& str);
        void config_read();
        //int read_commands(FILE * input);


    };


/*    
    class BMP 
    {
        private:
            int width;
            int height;
            unsigned char* data;
            unsigned char* header;

        public:
            BMP(int width, int height);
            void setPixel(int x, int y, unsigned char color);
            void save(const char* filename);
            ~BMP();
    };
*/ //не вошло в итоговый вариант, но хорошо бы описать пригодившиеся bmp структуры
}
    int maximum (int x, int y);
    int minimum (int x, int y);
    double modul (double x);


enum
{
    SUCCESS_command_receiving_place,
    FAIL_command_receiving_place,
    USAGE_wrong_command_input,
    SUCCESS_INIT,
    FAIL_INIT,
    SUCCESS_GSS,
    SUCCESS_GNU,
    GNU_fail_not_generated_field,
    SUCCESS_GEN,
    SUCCESS_BMP_WRITE,
    SUCCESS_BMP_READ,
    BMP_FILE_FOR_READING_NOT_FOUND,
    FAIL_BMP_opening,
    SECOND_FIELD_INITIALIZATION,
    FIELD_INIT_TRY_AFTER_BMP_READ,
    NO_bmp_reading_after_gausses,
    GAUSS_ADD_TRY_AFTER_BMP_READ,
    ERROR_UNINITIALIZED_FIELD,
    NO_FIELD_FOR_CUTTING,
    NO_FIELD_FOR_WAVE,
    IMPOSSIBLE_CUT_VALUE,
    BIN_SUCCESS,
    FAIL_creating_wave_slice,
    WAVE_Error_writing_bmp,
    SUCCESS_WAVE,
    //ERROR_WAVE,
    FAIL_bin_matrix_init,
    NO_POINTS_FOR_CLUSTERING,
    SUCCESS_kmeans,
    SUCCESS_KMCORES,
    NO_FIELD_FOR_KMEANS,
    NO_FIELD_FOR_KMCORES,
    GSS,
    INIT,
    GNU,
    GEN,
};


#endif // group_212_Chernyshova_Svetlana_Gauss_functions