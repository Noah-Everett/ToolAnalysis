//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                       THistReader.h                                     //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*// Author:                                                                                 //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                                           //*//
//*//   Created ¯\_(ツ)_/¯                                                                    //*//
//*//                                                                                         //*//
//*// About:                                                                                  //*//
//*//   This file contains a general class for reading ROOT histograms.                       //*//
//*//                                                                                         //*//
//*// Notation:                                                                               //*//
//*//   t_ = function argument                                                                //*//
//*//   m_ = member variable                                                                  //*//
//*//   mainVariable_subCategory (use '_' like LaTeX)                                         //*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//

#ifndef THISTREADER_H
#define THISTREADER_H

#include <map>
#include <string>
#include <vector>
#include <ostream>
#include <memory>
#include <functional>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

using std::map;
using std::string;
using std::vector;
using std::cout;
using std::endl;

// Forward declaration
template< typename type_ID, typename type_hist >
class THistReader;

/**////////////////////////
/**/// GlobalFileManager///
/**////////////////////////
/**/
class GlobalFileManager {
public:
    static GlobalFileManager& getInstance() {
        static GlobalFileManager instance;
        return instance;
    }

    void addFile(TFile* file) {
        m_files.push_back(file);
    }

    template< typename type_ID, typename type_hist >
    void addHistMap(const map<type_ID, type_hist*>* histMap) {
        m_histMaps.push_back(std::make_pair(
            const_cast<void*>(static_cast<const void*>(histMap)),
            [](void* p) {
                auto* map = static_cast<const map<type_ID, type_hist*>*>(p);
                for (const auto& pair : *map) {
                    delete pair.second;
                }
            }
        ));
    }

    ~GlobalFileManager() {
        // Close all files
        for (auto file : m_files) {
            if (file) {
                file->Close();
                delete file;
            }
        }

        // Delete all histograms
        for (const auto& histMapPair : m_histMaps) {
            histMapPair.second(histMapPair.first);
        }
    }

private:
    GlobalFileManager() {}
    GlobalFileManager(const GlobalFileManager&) = delete;
    GlobalFileManager& operator=(const GlobalFileManager&) = delete;

    vector<TFile*> m_files;
    vector<std::pair<void*, std::function<void(void*)>>> m_histMaps;
};
/**/
/**////////////////////////

template< typename type_ID, typename type_hist >
class THistReader
{
public:
    /**////////////////////////
    /**/// (Con/De)structor ///
    /**////////////////////////
    /**/
    THistReader(const vector<string>& t_hists_paths, 
                const vector<type_ID>& t_hists_IDs,
                const vector<string>& t_hists_names);
    THistReader(const THistReader& other);
    THistReader(THistReader&& other) noexcept;
    ~THistReader() = default;
    /**/
    /**////////////////////////

    /**////////////////////////
    /**/// Access Functions ///
    /**////////////////////////
    /**/
    type_hist* get_hist(const type_ID& t_ID) const;
    const map<type_ID, type_hist*>& get_histsMap() const { return m_hists; }
    /**/
    /**////////////////////////

    /**/////////////////////////
    /**/// Setting Functions ///
    /**/////////////////////////
    /**/
    THistReader& operator=(const THistReader& other);
    THistReader& operator=(THistReader&& other) noexcept;
    /**/
    /**/////////////////////////

private:
    /**////////////////////////
    /**/// Member Variables ///
    /**////////////////////////
    /**/
    map<type_ID, type_hist*> m_hists;
    /**/
    /**////////////////////////
};

/**////////////////////////////
/**/// Function Definitions ///
/**////////////////////////////
/**/
template< typename type_ID, typename type_hist >
THistReader< type_ID, type_hist >::THistReader(const vector<string>& t_hists_paths, 
                                               const vector<type_ID>& t_hists_IDs,
                                               const vector<string>& t_hists_names) 
{
    if (t_hists_paths.size() != t_hists_IDs.size() || t_hists_paths.size() != t_hists_names.size()) {
        cout << "Error: hists_paths, hists_IDs, and hists_names "
             << "vectors should all be the same length" << endl;
        return;
    }

    for (size_t i = 0; i < t_hists_paths.size(); i++) {
        TFile* file = new TFile(t_hists_paths[i].c_str(), "READ");
        GlobalFileManager::getInstance().addFile(file);
        
        if (file->IsZombie()) {
            cout << "Error: Could not open file with path " << t_hists_paths[i] << endl;
            continue;
        }

        type_hist* hist = nullptr;
        file->GetObject(t_hists_names[i].c_str(), hist);
        if (!hist) {
            cout << "Error: Could not find histogram with name " << t_hists_names[i] << endl;
            continue;
        }

        m_hists[t_hists_IDs[i]] = hist;
    }

    GlobalFileManager::getInstance().addHistMap(&m_hists);
}

template< typename type_ID, typename type_hist >
THistReader< type_ID, type_hist >::THistReader(const THistReader& other) : m_hists(other.m_hists) {
    GlobalFileManager::getInstance().addHistMap(&m_hists);
}

template< typename type_ID, typename type_hist >
THistReader< type_ID, type_hist >::THistReader(THistReader&& other) noexcept : m_hists(std::move(other.m_hists)) {
    GlobalFileManager::getInstance().addHistMap(&m_hists);
}

template< typename type_ID, typename type_hist >
THistReader< type_ID, type_hist >& THistReader< type_ID, type_hist >::operator=(const THistReader& other) {
    if (this != &other) {
        m_hists = other.m_hists;
        GlobalFileManager::getInstance().addHistMap(&m_hists);
    }
    return *this;
}

template< typename type_ID, typename type_hist >
THistReader< type_ID, type_hist >& THistReader< type_ID, type_hist >::operator=(THistReader&& other) noexcept {
    if (this != &other) {
        m_hists = std::move(other.m_hists);
        GlobalFileManager::getInstance().addHistMap(&m_hists);
    }
    return *this;
}

template< typename type_ID, typename type_hist >
type_hist* THistReader< type_ID, type_hist >::get_hist(const type_ID& t_ID) const { 
    auto it = m_hists.find(t_ID);
    if (it != m_hists.end()) {
        return it->second;
    }
    cout << "Error: Histogram with ID " << t_ID << " not found." << endl;
    return nullptr;
}
/**/
/**////////////////////////////

#endif