//112-Chernyshova-Svetlana-finding the minimum value - main
#include "group_212_Chernyshova_Svetlana_Gauss_functions.h"
using namespace Gauss_program;
int main(void)
//112-Chernyshova-Svetlana
//starting program; this main function has nothing on the entrance and returns integer; 
//it talks to the person and reads the segment, also checks wheather the number is really an integral
{
    Interface interface_obj;
    //std::string interface_log_filename;

/*
// значения по умолчанию
    std::string Batch_filename;
    int Field_DEF_x_size, Field_DEF_y_size;
    int Gauss_DEF_hight, Gauss_DEF_mu_x, Gauss_DEF_mu_y, Gauss_DEF_sigma_x, Gauss_DEF_sigma_y;
    const char *BMP_creation_filename;
    char *BMP_reading_filename;
    char *Gnuplot_data_filename;
    char *Gnuplot_commands_filename;
    int BIN_hight;
    int WAVE_min_component_size;
    int KMEANS_amount_of_dots;
    int KMCORE_amount_of_cores, KMCORE_amount_of_dots_in_cores;
*/

    printf("212-Chernyshova-Svetlana-Gauss.\n");
    printf("We are going to make a field of gaussians.\n");

    //interface_obj.config_read(interface_obj.interface_log_filename);
    interface_obj.config_read();


    //char* control_log_filename = "log_control.txt";
    
    //char* interface_log_filename = "log_interface.txt";
    char get_command[MAX_AMOUNT]; // = {"0"};
    //int arr[4] = {0,2,3,4};
    //for (int i = 0; i < 4; i++) printf("%d ",arr[i]);
    //printf("\n");
    //char *get_command = nullptr;
    FILE * input = nullptr;
    int feedback;
    ///Log_control_file log_control_file_obj;
    char * estr; //error str

    feedback = interface_obj.command_receiving_place(&input);
    //printf("6\n");
    if (feedback == FAIL_command_receiving_place) return -1;
    //printf("7\n");
    //feedback = interface_obj.read_commands(input);
    //if ()
/*
    switch (feedback)
    {
        case FAIL_command_receiving_place:
            return -1;
        //default:
    }
*/

/*
    printf("Put in the command - > ");
    fgets(get_command, N, stdin);

    if (get_command[0] == 'g')
    {
        gauss[0] = set_data()
    }
*/
    
//    printf("%s\n", get_command);
//    printf("10\n");
    //problem: segm fault
//    fgets(get_command, sizeof(char) * MAX_AMOUNT, input);
    //input = fopen("test1.txt","r");
/*
    if (fscanf(input, "%s", get_command) != 1)
    {
        printf ("There is no any command on the input.\n");
        return -1;
    }
*/

    //printf("9\n");
/*
    int k = 0;
    fscanf(input, "%s %d", get_command, &k);
    printf("%d %s\n", k, get_command);
    return -1;
*/
    estr = fgets(get_command, MAX_AMOUNT, input);
    //while (fscanf(input, "%s", get_command) == 1) // пока не вернул nullptr из-за ошибки или конца файла
    while ((estr != nullptr) && (estr[0] != '\0'))
    {
        
        //printf("8\n");
        feedback = interface_obj.understand_command(get_command);
        //printf("222\n");

        ////log_control_file_obj.feedback_printing(feedback);
        char arrr[MAX_AMOUNT];
        //printf("check-main-1\n");
        strcpy(arrr, interface_obj.interface_log_filename.c_str()); // куда копируем + что копируем
        ///printf("check-main-2\n");
        //interface_obj.interface_logging.feedback_printing(interface_obj.interface_logging, feedback); //общие логи о том что сделано. печать в интерфейсе
        interface_obj.interface_logging.feedback_printing(feedback); //общие логи о том что сделано. печать в интерфейсе
        ///printf("check-main-3\n");

        //printf("777\n");
        estr = fgets(get_command, MAX_AMOUNT, input);
        //printf("5555\n");
    }
    printf("There are no more commands in the file. Everything is complete.\n");
    fclose(input);
    return 0;
}
