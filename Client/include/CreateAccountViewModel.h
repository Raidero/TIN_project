#ifndef CREATEACCOUNTVIEWMODEL_H
#define CREATEACCOUNTVIEWMODEL_H
#include "LoginViewModel.h"

class CreateAccountViewModel: public LoginViewModel
{
    public:
        CreateAccountViewModel(ViewModel* mvm);
        virtual ~CreateAccountViewModel();
        void buttonPressed(int i);
        void refresh(int message);
};

#endif // CREATEACCOUNTVIEWMODEL_H
