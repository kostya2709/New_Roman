#include "Front_Head.h"

Node* Tree::Find_Derive (Node* node1)
{
#include "DEFINES.h"

Node* answer = NULL;

        switch (node1->node_type)
        {
            case OPERATOR:
            {
                switch ((int)N_DATA)
                {
                    case (MIN):
                    {
                        answer = dMIN (dL, dR);
                        break;
                    }
                    case (ADD):
                    {
                        answer = dSUM (dL, dR);
                        break;
                    }

                    case (MUL):
                    {

                        answer = dSUM (dMUL(dL, cR), dMUL (cL, dR));
                        break;
                    }
                    case (DIV):
                    {
                        answer = dDIV (dMIN (dMUL (dL, cR), dMUL (cL, dR)), dSQR(cR));
                        break;
                    }
                    case (POW):
                    {
                        if (node1->right->node_type == NUMBER)
                            answer = dMUL ( dMUL (dPOW (cL, CONST(R_DATA - 1)), cR), cL);
                        else
                            answer = dMUL (id, dMUL (dLN (cL), cR));
                        break;
                    }
                    case (LN):
                    {
                        answer = dMUL (dDIV (CONST(1), cR), cR);
                        break;
                    }
                    case (LOG):
                    {
                        answer = dDIV (dLN (cL), dLN (cR));
                        break;
                    }
                    case (LG):
                    {
                        answer = dDIV (dLN (cR), dLN (CONST (10)));
                        break;
                    }
                    case (SIN):
                    {
                        answer = dMUL (dCOS, cR);
                        break;
                    }
                    case (COS):
                    {
                        answer = dMUL ( dMUL ( dSIN, CONST (-1)), cR);
                        break;
                    }
                    case (TAN):
                    {
                        answer = dMUL (dDIV (CONST (1), dSQR (dCOS) ), cR);
                        break;
                    }
                    case (COTAN):
                    {
                        answer = dMUL ( dDIV ( CONST (-1), dSQR (dSIN)), cR);
                        break;
                    }
                    case (SH):
                    {
                        answer = dMUL (dCH, cR);
                        break;
                    }
                    case (CH):
                    {
                        answer = dMUL (dSH, cR);
                        break;
                    }
                    case (TH):
                    {
                        answer = dMUL (dDIV (CONST (1), dSQR (dCH)), cR);
                        break;
                    }
                    case (CTH):
                    {
                        answer = dMUL (dDIV (CONST (-1), dSQR (dSH)), cR);
                        break;
                    }

                    default:
                    {
                        printf ("Unidentified operator " ACCUR "!\n", N_DATA);
                        break;
                    }

                }

                break;
            }

            case NUMBER:
            {
                N_DATA = 0;
                break;
            }

            case VAR:
            {
                node1->node_type = NUMBER;
                if (strcmp (node1->sym, "Цезарь") == 0)
                    N_DATA = 1;
                else
                    N_DATA = 0;

                break;
            }

            default:
            {
                printf ("Unidentified node_type %d!\n", node1->node_type);
                break;
            }
        }

    if (N_DATA == LOG || N_DATA == LG)
        answer = Find_Derive (answer);


    switch ((int)N_DATA)
    {
        case POW: case LN: case SIN: case COS: case TAN: case COTAN:
        case SH: case CH: case TH: case CTH:
        answer->right = Find_Derive (answer->right);
        break;
    }

    if (answer)
    {
        if (node1->parent)
            if (node1->parent->left == node1)
                this->Insert_Node (node1->parent, answer, 0);
            else this->Insert_Node (node1->parent, answer, 1);
        else this->Insert_Node (answer);

        return answer;
    }

#include "UNDEFS.h"

    return node1;

}

