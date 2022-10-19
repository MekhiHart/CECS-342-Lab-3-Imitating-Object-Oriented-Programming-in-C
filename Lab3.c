#include <stdio.h> 
#include <stdlib.h> 
 
struct Employee { 
    void **vtable; 
    int age; 
}; 
 
struct HourlyEmployee {  // !
    void **vtable; // * contains functions
    int age; 
    double hourly_rate, hours;

 
}; 
 
struct CommissionEmployee { 
    void **vtable; // * contains functions
    int age; 
    double sales_amount;

}; 
 
struct SeniorSalesman { 
    void **vtable; // * contains functions
    int age; 
    double sales_amount;
    // overides get pay method
    // * uses speak commision employee's version of speak
    // also needs a constructor
    // also have a GetPay method that returns 20% of the salesman's sales, plus 50,000, plus another 5% of sales if the employee is at least 40 years old.
}; 
 
 
void Speak_Hourly(struct Employee *employee) { 
    struct HourlyEmployee *hourlyEmployee = (struct HourlyEmployee *) employee; 
    //Display hourly rate 
    printf("I work for %.2lf dollars per hour\n", hourlyEmployee->hourly_rate); 
} 
 
double GetPay_Hourly(struct Employee *employee) { 
    struct HourlyEmployee *hourlyEmployee = (struct HourlyEmployee * ) employee;
    return hourlyEmployee->hourly_rate * hourlyEmployee->hours;
} 

// void Construct_Hourly(struct HourlyEmployee *hourlyEmployee){
//     hourlyEmployee->age = 0;
//     hourlyEmployee->hourly_rate=0;
//     hourlyEmployee->hours = 0;
//     hourlyEmployee->vtable = Vtable_HourlyEmployee;
// }


 
void Speak_Commission(struct Employee *employee) { 
    struct CommissionEmployee *commissionEmployee = (struct CommissionEmployee *) employee; 
///Display sales amount 
    printf("I made a commission on %.2lf dollars from sales!\n", commissionEmployee->sales_amount); 
} 
 
double GetPay_Commission(struct Employee *employee) { // for commmision class
    struct CommissionEmployee *commissionEmployee = (struct CommisionEmployee *) employee;
    return (commissionEmployee->sales_amount * .10 ) + 40000;
} 
 
double GetPay_Senior(struct Employee *employee) {  // for senior class
    struct SeniorSalesman *seniorSalesman = (struct SeniorSalesman *) employee;
    double payPercentage = seniorSalesman->age >= 40 ? .25 : .20;
    int sales = seniorSalesman->sales_amount;
    return (payPercentage * sales) + 50000;

    
} 
 
void *Vtable_HourlyEmployee[2] = {Speak_Hourly, GetPay_Hourly};  // * vtable points to two functions
void *Vtable_CommissionEmployee[2] = {Speak_Commission,GetPay_Commission};
void *Vtable_SeniorSalesman[2] = {Speak_Commission,GetPay_Senior};
 
 
struct HourlyEmployee Construct_Hourly(struct HourlyEmployee *hourlyEmployee) {  // constructor for hourly employee
    hourlyEmployee->vtable = Vtable_HourlyEmployee; // ! vtable is a pointer which points to a pointer to Vtable_HourlyEmployee which is also a pointer
 
    hourlyEmployee->age = 0; 
    hourlyEmployee->hourly_rate = 0; 
    hourlyEmployee->hours = 0; 
} 
 
struct CommissionEmployee Construct_Commission(struct CommissionEmployee *commissionEmployee) { 
    commissionEmployee->vtable=Vtable_CommissionEmployee;
    commissionEmployee->age = 0;
    commissionEmployee->sales_amount=0;
} 
 
struct SeniorSalesman Construct_Senior(struct SeniorSalesman *seniorSalesman) { 
    seniorSalesman->vtable=Vtable_SeniorSalesman;
    seniorSalesman->age=0;
    seniorSalesman->sales_amount=0;
} 
 
 
 
int main() { 
    struct Employee *employee; 
    int userInput;
    printf("Choose an employee\n(1) Hourly Employee\n(2) Commision Employee\n(3) Senior Salesman\n");
    scanf("%d",&userInput);

    int age; 
    double pay;
    printf("How old is the employee?\n");
    scanf("%d",&age);

    switch (userInput)
    {
    case 1: // Hourly
        printf("Hourly\n");
        struct HourlyEmployee *pHourlyEmployee = (struct HourlyEmployee *) malloc(sizeof(struct HourlyEmployee));
        double employeeRate,employeeHours;
        Construct_Hourly(pHourlyEmployee);

        printf("What is the employee's pay rate?\n");
        scanf("%lf",&employeeRate);
        printf("What is the employee's total hours?\n");
        scanf("%lf", &employeeHours);

        pHourlyEmployee->hourly_rate = employeeRate;
        pHourlyEmployee->hours = employeeHours;
        pHourlyEmployee->age = age; 
        employee = (struct Employee *) pHourlyEmployee;
        
        ((void (*)(struct Employee *)) employee->vtable[0])((struct Employee *) employee); // * invoking speak for hourly employee
        pay = ((double (*)(struct Employee *)) employee->vtable[1])((struct Employee *) employee); // * invoking get pay for hourly employee
        break;

    default: // 
        double employeeSales;
        printf("What was the employee's amount of sales?\n");
        scanf("%lf",&employeeSales);

        if (userInput == 2){ // Commision
            printf("Commision\n");
            struct CommissionEmployee *pCommisionEmployee = (struct CommissionEmployee * ) malloc(sizeof(struct CommissionEmployee));
            Construct_Commission(pCommisionEmployee);
            pCommisionEmployee->sales_amount = employeeSales;
            pCommisionEmployee->age = age;
            employee = (struct Employee *) pCommisionEmployee;

        
            ((void (*)(struct CommissionEmployee  *)) employee->vtable[0])((struct CommissionEmployee  *) employee);
            pay = ((double (*)(struct CommissionEmployee  *)) employee->vtable[1])((struct CommissionEmployee  *) employee);
        }
        else{ // Senior
            printf("Senior\n");
            struct SeniorSalesman *pSeniorSalesman = (struct SeniorSalesman * ) malloc(sizeof(struct SeniorSalesman));
            Construct_Senior(pSeniorSalesman);
            pSeniorSalesman->sales_amount = employeeSales;
            pSeniorSalesman->age = age;
            employee = (struct Employee *) pSeniorSalesman;
        
            // function invocation
            // (*) tells C compiler that the pointer points to an a function. Without it, it represents just a 
            // the paramter is ((struct SeniorSalesman  *) employee)
            ((void (*)(struct SeniorSalesman  *)) employee->vtable[0]) ((struct SeniorSalesman  *) employee);
            pay = ((double (*)(struct SeniorSalesman  *)) employee->vtable[1])((struct SeniorSalesman  *) employee);
        }
        break;
    }
    printf("Employee Pay: %lf\n", pay);

    return 0; 
} 
 