void quickSort(int* arr, const int left, const int right) {
   int i = left, j = right;
   int aux;
 
   int pivot = arr[(left + right) / 2];
   while (i <= j)
   {
      while (arr[i] < pivot) i++;
      while (arr[j] > pivot) j--;
      if (i <= j)
      {
         aux = arr[i];
         arr[i] = arr[j];
         arr[j] = aux;
         i++;
         j--;
      }
   };
 
   if (left < j)
      quickSort(arr, left, j);
   if (i < right)
      quickSort(arr, i, right);
}
