#include "CData.h"


#include "CsvWriter.h"

//------------------------- AddData -------------------------------------
//
//  adds a new pattern to data
//-----------------------------------------------------------------------
void CData::AddData(vector<double> &data, vector<double> &targets)
{
	CsvWriter writer("data.csv");

	if (data.size() == nb_input && targets.size() == nb_target) {
		m_vecInputs.push_back(data);
		m_vecTargets.push_back(targets);
		writer.addDatainRow(data.begin(), data.end());
	}
	else {
		throw std::exception("Please check Input & Target Data !");
	}

}

double CData::GetNbShooting()
{
	
	double temp = 0;

	if (m_vecTargets.size() > 0) {

		for (size_t i = 0; i < nb_target; i++)
		{
			if (m_vecTargets[i][0] == 1)
				temp++;
		}
		return temp / m_vecTargets.size();

	}
	
	return temp;
}

