#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>

//Функция проверки типа данных
void checkFormat(float *num){
  char buffer[300];
  do{
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%f", num) != 1) printf("Ошибка ввода данных.\nПопробуйте снова: ");
  } while (sscanf(buffer, "%f", num) != 1);
}

//Функция проверки диапазона
void checkType(float *num){
  if (errno == ERANGE){
    printf("Выход за диапазон типа. Программа предварительно завершена.\n");
    exit(1);
  }
}

//Функция сортировки
void gsort(float x[], float y[], int size){

    int i = 1;
    int j = 2;
    float temp;

    //Пока не достигнут последний элемент массива
    while(i < size){

      //Если порядок правильный, переходим дальше на "один шаг"
      if (x[i - 1] < x[i]){

        i = j;
        j++;

      }else{

        //Иначе меняем местами
        temp = x[i - 1];
        x[i - 1] = x[i];
        x[i] = temp;
        temp = y[i - 1];
        y[i - 1] = y[i];
        y[i] = temp;

        //"Назад на шаг"
        i = i - 1;
        //Если достигли начала массива, то возвращаемся
        if (i == 0){
          i = j;
          j = j + 1;
        }

      }

    }
}

float LegPol(int n, float x){
  float z;
  if(n == 0){
    return 1;
  }else if(n == 1){
    return x;
  }else{
    z = (2*n + 1)*x*LegPol(n-1, x)/(n+1) - n*LegPol(n-2, x)/(n+1);
    return z;
  }
}

int main() {

  float x[256]; float y[256];
  int i = 0;

  //Начало кода сортировки
  //Проверка на наличие файла
  printf("Запуск модуля сортировки...\n");
  if (access("input.txt", R_OK) != 0){
    printf("Файл input.txt отсутствует. Предварительное завершение программы.");
    exit(1);
  }else{
    FILE *input = fopen("input.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char buffer[256];

    //Ввод с файла с проверкой на тип и диапазон
    while(!feof(input)){
      if (fgets(buffer, sizeof(buffer), input) == NULL){
        break;
      }
      if (sscanf(buffer, "%f %f", &x[i], &y[i]) != 2){
        printf("Ошибка ввода данных. Предварительное завершение программы.");
        exit(1);
      }

      checkType(&x[i]);
      checkType(&y[i]);
      i++;
    }

    //Сортировка
    gsort(x, y, i);
    for (int j = 0; j < i; j++){
      //Проверка повторяющихся значений
      if (x[j] == x[j+1] && y[j] != y[j+1]){
        printf("Содержатся элементы с одинаковыми значениями x. Программа предварительно завершена.");
        fclose(input);
        fclose(temp);
        exit(1);
      }else{
        //Запись в файл
        fprintf(temp, "%.2f %.2f\n", j, x[j], j, y[j]);
      }
    }
    fclose(input);
    fclose(temp);
  }
  printf("Сортировка успешно выполнена!\n\n");
  //Конец блока сортировки

  int t1 = i;
  printf("Введите максимальную степень полинома (от 2 до %d): ", i);
  scanf("%d", &i);
  getchar();

  printf("\nЗапуск модуля интерполяции...\n\nЗначения созданной матрицы:\n");
  //Начало блока интерполяции
  double matrix[i][i+1];

  for (int j = 0; j < i; j++){
    for (int k = 0; k < i+1; k++){

      matrix[j][k] = 2;
      double sum = 0;

      for (int m = 0; m < i; m++){
        double temp = 0;
        temp += LegPol(j, x[m]);
        if (k == i){
          temp *= y[m];
        }else{
          temp *= LegPol(k, x[m]);
        }
        sum += temp;
      }

      matrix[j][k] *= sum;

      printf("\t%.2lf ", matrix[j][k]);
    }
    printf("\n");
  }

  printf("\n");

  double b[i];
  for (int j = 0; j < i; j++){
    b[j] = matrix[j][i];
  }
  int j, k, m;
  double a[i];
  double temp=0;

  for(m = 0; m < i; m++){
   for(j = m + 1; j < i; j++){
     double M = matrix[j][m]/matrix[m][m];
     for(k = m; k < i; k++){
       matrix[j][k] -= M * matrix[m][k];
     }
     b[j] -= M*b[m];
   }
  }
  for(j = i - 1; j >= 0; j--){
    float s = 0;
    for(k = j; k < i; k++){
      s = s + matrix[j][k]*a[k];
    }
    a[j] = (b[j] - s) / matrix[j][j];
    printf("%lf\n", a[j]);
  }


  float X;
  double Y = 0;
  printf("Введите x в диапазоне от %5.2f до %5.2f: ", x[0], x[t1-1]);
  checkType(&X);
  checkFormat(&X);
  printf("\n");
  if (X <= x[0] || X >= x[t1-1]){
    printf("Введенное x не соответствует допустимому диапазону. Предварительное завершение программы.");
    exit(1);
  }

  for (int j = 0; j < i; j++){
    Y += a[j]*LegPol(j, X);
  }

  printf("f(%4.2f) = %4.2lf\n\n", X, Y);
  printf("Интерполяция успешно выполнена!\n\nЗавершение работы программы.");

  return 0;
}
