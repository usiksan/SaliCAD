#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>

#include <QCoreApplication>
#include <QDebug>

const char *prog =
R"VVV(
def multiply(a,b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c
)VVV";


int main(int argc, char *argv[])
  {

  QCoreApplication a(argc, argv);

  PyObject *pModule, *pFunc;
  PyObject *pArgs, *pValue;

  Py_Initialize();
  qDebug() << "init ok";

  PyObject *pGlobal = PyDict_New();
  PyObject *pLocal  = PyDict_New();

  pModule = PyRun_String( prog, Py_single_input, pGlobal, pLocal );

  PyObject_Print( pLocal, stderr, 0 );

  if (pModule != NULL) {
      pFunc = PyDict_GetItemString( pLocal, "multiply" );
      /* pFunc is a new reference */

      if (pFunc && PyCallable_Check(pFunc)) {
          pArgs = PyTuple_New(2);

          pValue = PyLong_FromLong(3);
          if (!pValue) {
              Py_DECREF(pArgs);
              Py_DECREF(pModule);
              fprintf(stderr, "Cannot convert argument\n");
              return 1;
          }
          PyTuple_SetItem(pArgs, 0, pValue);


          pValue = PyLong_FromLong(6);
          if (!pValue) {
              Py_DECREF(pArgs);
              Py_DECREF(pModule);
              fprintf(stderr, "Cannot convert argument\n");
              return 1;
          }
          PyTuple_SetItem(pArgs, 1, pValue);

          pValue = PyObject_CallObject(pFunc, pArgs);
          Py_DECREF(pArgs);
          if (pValue != NULL) {
              printf("Result of call: %ld\n", PyLong_AsLong(pValue));
              Py_DECREF(pValue);
          }
          else {
              Py_DECREF(pFunc);
              Py_DECREF(pModule);
              PyErr_Print();
              fprintf(stderr,"Call failed\n");
              return 1;
          }
      }
      else {
          if (PyErr_Occurred())
              PyErr_Print();
          fprintf(stderr, "Cannot find function \"%s\"\n", "1");
      }
      Py_XDECREF(pFunc);
      Py_DECREF(pModule);
  }
  else {
      PyErr_Print();
      fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
      return 1;
  }
  if (Py_FinalizeEx() < 0) {
      return 120;
  }
  return 0;

  return a.exec();
  }
