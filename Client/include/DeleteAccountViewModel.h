#ifndef DELETEACCOUNTVIEWMODEL_H
#define DELETEACCOUNTVIEWMODEL_H
#include "LoginViewModel.h"

class DeleteAccountViewModel: public LoginViewModel
{
public:
    DeleteAccountViewModel(ViewModel* mvm);
    virtual ~DeleteAccountViewModel();
    void buttonPressed(int i);
    void refresh(int message);
};

#endif // DELETEACCOUNTVIEWMODEL_H
