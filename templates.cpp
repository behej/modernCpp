#include <iostream>
#include <vector>
#include <string>
#include <array>

using namespace std;

/* TEMPLATE DE FONCTION */
/*----------------------*/

/* On déclare une fonction générique sans spécifier le type. On indique simplement
qu'il s'agit d'un template: une fonction appelée avec le type T.
On peut appeler cette fonction avec n'importe quel type. Les seules contraintes à respecter sont:
- a et b doivent être de même type
- l'opérateur + doit exister pour le type T
*/
template <typename T>
T sum(T a, T b)
{
    return (a + b);
}

/* Specialisation */
/*----------------*/

/* La fonction template peut égaement être spécialisée.
Ici la même fonction est redéfinie pour le type particulier 'string'.
L'implémentation peut être différente.
*/
template <>
string sum<string>(string a, string b)
{
    return (a + " - " + b);
}

/* TEMPLATE DE CLASSE */
/*--------------------*/

/* Le principe est exactement le même pour les classe.
Dans l'exemple c-dessous, la classe possède même 2 types différents non définis.
*/
template <typename T, typename U>
class MyClass
{
public:
    MyClass(T var1, U var2) : m_var1(var1), m_var2(var2) {}
    T getVar1() const { return m_var1; }
    U getVar2() const { return m_var2; }
    static string whoami()
    {
        return "I am general template class";
    }
    void execute() const
    {
        cout << "- Call operator() from template class -->" << m_var1() << endl;
    }

private:
    T m_var1;
    U m_var2;
};

/* Spécialisation */
/*----------------*/

/* On peut également spéciliser les classes template.
Ici, la même déclaration de classe mais avec la particularité
que les 2 paramètres sont du mếme type.
*/
template <typename T>
class MyClass<T, T>
{
public:
    MyClass(T var1, T var2) : m_var1(var1), m_var2(var2) {}
    T getVar1() const { return m_var1; }
    T getVar2() const { return m_var2; }
    static string whoami()
    {
        return "I am specialized template class";
    }
    void execute() const
    {
        cout << "Call operator() from template class --> " << m_var1() << endl;
    }

private:
    T m_var1;
    T m_var2;
};

class MyOperatorClass
{
public:
    string operator()() const
    {
        return "Operator() from class MyClassOperator";
    }
};

/* On peut indiquer des types par défaut à appliquer si le type n'est pas indiqué au
moment de l'instanciation ou s'il n'est pas possible de le déduire.

Les templates permettent aussi de spécifier des valeurs qui sont ensuite utilisée
dans la classe. On peut égakement y associer des valeurs par défaut.
 */
template <typename T = int, int size = 3, T default_value = (T)42>
class MyArrayClass
{
public:
    MyArrayClass(T initial_value)
    {
        m_values.fill(initial_value);
    }
    MyArrayClass()
    {
        m_values.fill(default_value);
    }
    void print()
    {
        for (const auto &value : m_values)
        {
            cout << value << " ";
        }
        cout << endl;
    }

    void odd_or_even()
    {
        /* COMPILATION CONDITIONNELLE */
        /*----------------------------*/

        /* Compilation conditionnelle (C++17)
        La condition 'if constexpr' est évaluée au moment de la compilation.
        Cela implique que le test ne peut se faire que sur des données connues au moment de la compil
        Cela implique aussi que seul une partie du code est compilée et donc embarquée réellement
        dans le binaire.
        */
        if constexpr (size % 2)
        {
            cout << "I'm odd" << endl;
        }
        else
        {
            cout << "I'm even" << endl;
        }
    }

private:
    array<T, size> m_values;
};

/* TEMPLATE TEMPLATE */
/*-------------------*/

template <typename T>
class MyTemplateClass
{
public:
    MyTemplateClass(T initial_value, int size = 5)
    {
        m_values.reserve(size);
        for (int i = 0; i < size; i++)
        {
            m_values.push_back(initial_value);
        }
    }
    void print()
    {
        cout << "MyTemplateClass:" << endl
             << "-> ";
        for (const auto &value : m_values)
        {
            cout << value << " ";
        }
        cout << endl;
    }

private:
    vector<T> m_values;
};

/* On peut également définir des template de template.
L'utilisation n'est pas courant et sera surtout utilisée pour le pattern "Policy based design".
 */
template <typename T, template <typename> class Policy = MyTemplateClass>
class MyTemplateTemplateClass
{
public:
    explicit MyTemplateTemplateClass(const Policy<T> &policy = MyTemplateClass<T>()) : m_policy(policy) {}
    void run()
    {
        m_policy.print();
    }

private:
    Policy<T> m_policy;
};

/* VARIADIC TEMPLATE */
/*-------------------*/

/* Définition d'une classe template d'un nombre variable de types */
template <typename... Types>
class MyVariadicTemplateClass
{
};

/* Classe template avec le nombre minimal de paramètres (ici 0)
 */
template <>
class MyVariadicTemplateClass<>
{
public:
    MyVariadicTemplateClass()
    {
        cout << "No more types" << endl;
    }
};

/* Classe template avec définition récursive.
Le premier type est utilisé et les suivants sont passés de manière récursive
pour rappeler à nouveau la classe template. La récursivité est effectuée grâce
à l'héritage. On termine avec la classe avec 0 paramètres ci-dessus.
NOTE:
Le constructeur appelle d'abord le constructeur de la classe de base avant d'éxécuter
le code. C'est pourquoi on va d'abord traiter les derniers types.
*/
template <typename T, typename... Types>
class MyVariadicTemplateClass<T, Types...> : MyVariadicTemplateClass<Types...>
{
public:
    MyVariadicTemplateClass(T var1, Types... args) : MyVariadicTemplateClass<Types...>(args...)
    {
        cout << var1 << endl;
    }
};

int main()
{
    /* TEMPLATE DE FONCTIONS */
    /*-----------------------*/

    // Syntaxe explicite: on indique le type à utiliser dans le template.
    // PROS: le cast de l'int 3.1 en int est réalisé automatiquement.
    cout << "Function template" << endl;
    cout << "-----------------" << endl;
    cout << "- Call of a template function with explicit types:" << endl;
    cout << "  3.1 + 4 (as ints) = " << sum<int>(3.1, 4) << endl;

    // Syntaxe implicite: le type est automatiquement déduit
    // CONS: pas de cast implicit (car pas possible)
    cout << "- Call of a template function with implicit types:" << endl;
    cout << "  4 + 3.5 = " << sum(4., 3.5) << endl;

    // Appel de la fonction template spécialisée pour les string
    cout << "- Call of specialized template function" << endl;
    cout << "  " << sum(string(" Hello "), string(" world ")) << endl;

    /* TEMPLATE DE CLASSES */
    /*---------------------*/

    // fonctionne également avec les template de classe
    // (déduction automatique des types depuis C++17)
    cout << endl
         << "Class template" << endl;
    cout << "--------------" << endl;

    MyClass<int, float>
        foo(3, 4.2);
    MyClass bar("Hello", "World");
    cout << "- Object foo: var1=" << foo.getVar1() << " ; var2=" << foo.getVar2() << endl;
    cout << "- Object bar: " << bar.getVar1() << " / " << bar.getVar2() << endl;

    cout << "Template specilisation" << endl;
    // Instanciation du template générique
    cout << "- Different types -> generic template: " << MyClass<int, double>::whoami() << endl;
    // Instanciation du template générique
    cout << "- Same type -> specialized template: " << MyClass<int, int>::whoami() << endl;

    /*   OPERATOR()   */
    /*----------------*/

    // Ici on définit une classe qui possède un opérateur ()
    // On peut appeler cet opérateur directement depuis un object de cette classe.
    auto A{MyOperatorClass()};
    cout << "Operator() on typename within template class/function" << endl;
    cout << "- Direct call to operator() on object of type MyOperatorClass --> " << A() << endl;

    // Mais on peut aussi utiliser cette classe en tant que type du template
    MyClass baz(MyOperatorClass(), 3);
    baz.execute();

    /*  VALEURS PAR DEFAUT  */
    /*----------------------*/

    // Instanciation de classes template avec des valeurs par défaut
    MyArrayClass array1;
    MyArrayClass<double, 4> array2(3);
    MyArrayClass array3(3.5);

    cout << endl
         << "Default values for template" << endl;
    cout << "---------------------------" << endl;
    cout << "- default type + default value + default parameter: ";
    array1.print();
    cout << "- explicit type + value + parameter: ";
    array2.print();
    cout << "- deduced type + default value + explicit parameter: ";
    array3.print();

    cout << endl
         << "Conditionnal compilation" << endl;
    cout << "------------------------" << endl;
    cout << "This code only exist in object 'array2', not in 'array3': ";
    array2.odd_or_even();
    cout << "This code only exist in object 'array3', not in 'array2': ";
    array3.odd_or_even();

    /* TEMPLATE TEMPLATE */
    /*-------------------*/

    cout << endl
         << "Template template" << endl;
    cout << "-----------------" << endl;

    MyTemplateTemplateClass<int, MyTemplateClass> template_template1(MyTemplateClass<int>(3));
    template_template1.run();

    /* VARIADIC TEMPLATE */
    /*-------------------*/

    cout << endl
         << "Variadic templates" << endl;
    cout << "------------------" << endl;
    cout << "Build an object of variable types (number and diversity)" << endl;
    MyVariadicTemplateClass<int, double, string> variadic(3, 4.2, string("Hello"));
}

/**
 * Tempaltes:
 * + class templates
 * + method template
 * + default param
 * - decltype
 * + if constexpr
 * + template avec operateur ()
 * + specialisation
 * + template template
 * - variadic templates
 *
 */