#include "TBox.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TEllipse.h"
#include "TH2D.h"
#include "TLine.h"
#include "TPaletteAxis.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TString.h"
#include "TText.h"

#include<vector>

namespace sndisplay
{
  ////////////////////////////
  // sndisplay::calorimeter //
  ////////////////////////////

  class calorimeter
  {
  public:
    calorimeter (const char *n = "", bool with_palette=false) : calorimeter_name (n)
    {
      canvas_it = nullptr;
      canvas_fr = nullptr;

      // draw by default the OM ID
      draw_omid = true;
      draw_omnum = false;
      draw_content = false;
      draw_content_format = "%.0f";

      for (int omnum=0; omnum<nb_om; ++omnum)
	content.push_back(0);

      range_min = range_max = -1;

      const double spacerx = with_palette ? 0.0093458 : 0.0100;
      const double spacery = 0.0125;

      const double mw_sizey = (1-4*spacery)/(13+2);
      const double gv_sizey = mw_sizey;
      const double xw_sizey = mw_sizey*13/16.;

      const double mw_sizex = with_palette ? (1-5*spacerx)/(20+4+1.5) : (1-4*spacerx)/(20+4);
      const double gv_sizex = mw_sizex*20./16.;
      const double xw_sizex = mw_sizex;
      
      //////////////////////////
      // MWALL initialisation //
      //////////////////////////

      for (int mw_side=0; mw_side<2; ++mw_side) {

	for (int mw_column=0; mw_column<20; ++mw_column) {

	  for (int mw_row=0; mw_row<13; ++mw_row) {

	    int omnum = mw_side*20*13 + mw_column*13 + mw_row;

	    double x1 = spacerx + 2*xw_sizex + spacerx; // + 0.5*mw_side;
	    x1 += (mw_side == 0) ? mw_sizex*(19-mw_column) : mw_sizex*(mw_column); // swap IT for external view

	    double y1 = spacery + gv_sizey + spacery + mw_sizey*(mw_row);
	    double x2 = x1 + mw_sizex;
	    double y2 = y1 + mw_sizey;

	    TBox box (x1, y1, x2, y2);
	    box.SetFillColor(0);
	    box.SetLineWidth(1);
	    ombox.push_back(box);

	    TString omid_string = Form("M:%1d.%d.%d", mw_side, mw_column, mw_row);
	    TText omid_text (x1+0.5*mw_sizex, y1+0.7*mw_sizey, omid_string);
	    omid_text.SetTextFont(42);
	    omid_text.SetTextSize(0.013);
	    omid_text.SetTextAlign(22);
	    omid_text_v.push_back(omid_text);

	    TString omnum_string = Form("%03d", omnum);
	    TText omnum_text (x1+0.5*mw_sizex, y1+0.7*mw_sizey, omnum_string);
	    omnum_text.SetTextFont(42);
	    omnum_text.SetTextSize(0.013);
	    omnum_text.SetTextAlign(22);
	    omnum_text_v.push_back(omnum_text);

	    TText content_text (x1+0.5*mw_sizex, y1+0.3*mw_sizey, "");
	    content_text.SetTextSize(0.02);
	    content_text.SetTextAlign(22);
	    content_text_v.push_back(content_text);
  
	  } // for mw_row

	} // for mw_column

      } // for mw_side

      //////////////////////////
      // XWALL initialisation //
      //////////////////////////

      for (int xw_side=0; xw_side<2; ++xw_side) {

	for (int xw_wall=0; xw_wall<2; ++xw_wall) {

	  for (int xw_column=0; xw_column<2; ++xw_column) {

	    for (int xw_row=0; xw_row<16; ++xw_row) {

	    int omnum = 520 + xw_side*2*2*16 + xw_wall*2*16 + xw_column*16 + xw_row;

	    double x1;
	    
	    switch (xw_side) {
	    case 0:
	      if (xw_wall == 0) // IT-MO
		x1 = spacerx + 2*xw_sizex + spacerx + 20*mw_sizex + spacerx + (1-xw_column)*xw_sizex;
	      else              // IT-TU
		x1 = spacerx + xw_sizex*xw_column;
	      break;
	      
	    case 1:
	      if (xw_wall == 0) // FR-MO
		// x1 = 0.5 + spacerx + xw_sizex*xw_column;
		x1 = spacerx + xw_sizex*xw_column;
	      else // FR-TU
		// x1 = 0.5 + spacerx + 2*xw_sizex + spacerx + 20*mw_sizex + spacerx + (1-xw_column)*xw_sizex;
		x1 = spacerx + 2*xw_sizex + spacerx + 20*mw_sizex + spacerx + (1-xw_column)*xw_sizex;
	      break;}
	      
	    double x2 = x1 + xw_sizex;
	    double y1 = spacery + gv_sizey + spacery + xw_sizey*(xw_row);
	    double y2 = spacery + gv_sizey + spacery + xw_sizey*(xw_row+1);

	    TBox box (x1, y1, x2, y2);
	    box.SetFillColor(0);
	    box.SetLineWidth(1);
	    ombox.push_back(box);

	    TString omid_string = Form("X:%1d.%1d.%1d.%d", xw_side, xw_wall, xw_column, xw_row);

	    TText omid_text (x1+0.5*mw_sizex, y1+0.7*xw_sizey, omid_string);
	    omid_text.SetTextFont(42);
	    omid_text.SetTextSize(0.013);
	    omid_text.SetTextAlign(22);
	    omid_text_v.push_back(omid_text);

	    TString omnum_string = Form("%03d", omnum);

	    TText omnum_text (x1+0.5*xw_sizex, y1+0.7*xw_sizey, omnum_string);
	    omnum_text.SetTextFont(42);
	    omnum_text.SetTextSize(0.013);
	    omnum_text.SetTextAlign(22);
	    omnum_text_v.push_back(omnum_text);

	    TText content_text (x1+0.5*xw_sizex, y1+0.3*xw_sizey, "");
	    content_text.SetTextSize(0.02);
	    content_text.SetTextAlign(22);
	    content_text_v.push_back(content_text);

	    } // for xw_row

	  } // for xw_column

	} // for xw_wall

      } // for xw_side

      //////////////////////////
      // GVETO initialisation //
      //////////////////////////
      
      for (int gv_side=0; gv_side<2; ++gv_side) {

	for (int gv_wall=0; gv_wall<2; ++gv_wall) {

	  for (int gv_column=0; gv_column<16; ++gv_column) {

	    int omnum = 520 + 128 + gv_side*2*16 + gv_wall*16 + gv_column;

	    double x1;

	    if (gv_side == 0) 
	      x1 = spacerx + 2*xw_sizex + spacerx + gv_sizex*(16-1-gv_column);
	    else
	      // x1 = 0.5 + spacerx + 2*xw_sizex + spacerx + gv_sizex*gv_column;
	      x1 = spacerx + 2*xw_sizex + spacerx + gv_sizex*gv_column;

	    double x2 = x1 + gv_sizex;
	    double y1 = spacery + gv_wall*(gv_sizey + spacery + 13*mw_sizey + spacery);
	    double y2 = y1 + gv_sizey;
	    
	    TBox box (x1, y1, x2, y2);
	    box.SetFillColor(0);
	    box.SetLineWidth(1);
	    ombox.push_back(box);

	    TString omid_string = Form("G:%1d.%1d.%d", gv_side, gv_wall, gv_column);

	    TText omid_text (x1+0.5*gv_sizex, y1+0.7*gv_sizey, omid_string);
	    omid_text.SetTextFont(42);
	    omid_text.SetTextSize(0.013);
	    omid_text.SetTextAlign(22);
	    omid_text_v.push_back(omid_text);

	    TString omnum_string = Form("%03d", omnum);

	    TText omnum_text (x1+0.5*gv_sizex, y1+0.7*gv_sizey, omnum_string);
	    omnum_text.SetTextFont(42);
	    omnum_text.SetTextSize(0.013);
	    omnum_text.SetTextAlign(22);
	    omnum_text_v.push_back(omnum_text);

	    TText content_text (x1+0.5*gv_sizex, y1+0.3*gv_sizey, "");
	    content_text.SetTextSize(0.02);
	    content_text.SetTextAlign(22);
	    content_text_v.push_back(content_text);

	  } // for gv_column

	} // for gv_wall 

      } // for gv_side

      label_it = new TText (spacerx+xw_sizex, spacery+gv_sizey+spacery+13*mw_sizey+spacery+0.5*gv_sizey, "  ITALY");
      label_it->SetTextSize(0.028);
      label_it->SetTextAlign(22);

      label_fr = new TText (spacerx+xw_sizex, spacery+gv_sizey+spacery+13*mw_sizey+spacery+0.5*gv_sizey, "FRANCE");
      label_fr->SetTextSize(0.028);
      label_fr->SetTextAlign(22);

      const Int_t nRGBs = 6;
      Double_t stops[nRGBs] = { 0.00, 0.20, 0.40, 0.60, 0.80, 1.00 };
      Double_t red[nRGBs]   = { 0.25, 0.00, 0.20, 1.00, 1.00, 0.90 };
      Double_t green[nRGBs] = { 0.25, 0.80, 1.00, 1.00, 0.80, 0.00 };
      Double_t blue[nRGBs]  = { 1.00, 1.00, 0.20, 0.00, 0.00, 0.00 };

      palette_index = TColor::CreateGradientColorTable(nRGBs, stops, red, green, blue, 100);

      palette_histo = nullptr;
      palette_axis = nullptr;

      if (with_palette)
	{
	  const double palette_sizey = mw_sizey*13;
	  const double palette_sizex = mw_sizex*1.5;

	  palette_histo = new TH2D(Form("%s_palette_histo",calorimeter_name.Data()), "", 1, 0, 1, 1, 0, 1);
	  palette_histo->GetZaxis()->SetNdivisions(509);
	  palette_histo->GetZaxis()->SetLabelSize(0.024);
	  palette_histo->GetZaxis()->SetLabelFont(62);
	  palette_histo->SetMinimum(range_min);
	  palette_histo->SetMaximum(range_max);
	  palette_histo->SetContour(100);

	  // the constructor TPaletteAxis(x1, y1, x2, y2, histo)
	  // crashing due to no canvas existing (gPad = nullptr)
	  palette_axis = new TPaletteAxis;
	  palette_axis->SetHistogram(palette_histo);
	  palette_axis->SetX1NDC(1-spacerx-palette_sizex*(5./6)); // position tuning
	  palette_axis->SetY1NDC(spacery+gv_sizey+spacery+palette_sizey/8.);
	  palette_axis->SetX2NDC(1-spacerx-palette_sizex*(3./6)); // position tuning
	  palette_axis->SetY2NDC(1-palette_axis->GetY1NDC());
	  palette_axis->SetY2NDC(1-palette_axis->GetY1NDC());
	}

    }; // calorimeter()

    ~calorimeter()
    {
      if (palette_axis) delete palette_axis;
      if (palette_histo) delete palette_histo;

      delete label_fr;
      delete label_it;

      delete canvas_fr;
      delete canvas_it;
    };
    
    static const int nmwall = 520;
    static const int nxwall = 128;
    static const int ngveto =  64;

    static const int nb_om  = 712;

    enum {
      AUTO,
      FULL,
      FULL_ITALY,
      FULL_FRANCE,
      MW_ITALY,
      MW_FRANCE,
      XW_MOUNTAIN,
      XW_TUNNEL};

    void setrange(float zmin, float zmax) 
    {
      range_min = zmin; range_max = zmax;
    }

    void draw_omid_label (bool draw=true) {
      draw_omid = draw;}

    void draw_omnum_label(bool draw=true) {
      draw_omnum = draw;}
 
    void draw_content_label(const char *format="%.0f") {
      draw_content_format = TString(format);
      draw_content = true;}

    void draw()
    {
      const int canvas_width  = palette_axis ? 1284 : 1200;
      const int canvas_height = 780;

      if (canvas_it == nullptr)
	{
	  canvas_it = new TCanvas (Form("%s_canvas_it",calorimeter_name.Data()), Form("%s (IT side)",calorimeter_name.Data()), canvas_width, canvas_height);

	  // force canvas exact size
	  int decoration_width = canvas_width - canvas_it->GetWw();
	  int decoration_height = canvas_height - canvas_it->GetWh();
	  canvas_it->SetWindowSize(canvas_width+decoration_width, canvas_height+decoration_height);

	  // preserve width/height ratio in case of resizing
	  canvas_it->SetFixedAspectRatio();
	}

      if (canvas_fr == nullptr)
	{
	  canvas_fr = new TCanvas (Form("%s_canvas_fr",calorimeter_name.Data()), Form("%s (FR side)",calorimeter_name.Data()), canvas_width, canvas_height);
	  canvas_fr->SetWindowSize(canvas_width+(canvas_width-canvas_fr->GetWw()), canvas_height+(canvas_height-canvas_fr->GetWh()));

	  // force canvas exact size
	  int decoration_width = canvas_width - canvas_fr->GetWw();
	  int decoration_height = canvas_height - canvas_fr->GetWh();
	  canvas_fr->SetWindowSize(canvas_width+decoration_width, canvas_height+decoration_height);

	  // preserve width/height ratio in case of resizing
	  canvas_fr->SetFixedAspectRatio();
	}

      if (draw_content && !text_was_set)
	{
	  for (int omnum=0; omnum<nb_om; ++omnum)
	    settext(omnum, Form(draw_content_format.Data(), content[omnum]));
	}

      /////////////
      // Draw IT //
      /////////////

      canvas_it->cd();
      canvas_it->SetEditable(true);
      
      int mw_side=0;
      for (int mw_column=0; mw_column<20; ++mw_column) {
	for (int mw_row=0; mw_row<13; ++mw_row) {
	  int id = mw_side*20*13 + mw_column*13 + mw_row;
	  ombox[id].Draw("l");
	  if (draw_omid)
	    omid_text_v[id].Draw();
	  else if (draw_omnum)
	    omnum_text_v[id].Draw();
	  if ((draw_content && content[id]!=0) || text_was_set)
	    content_text_v[id].Draw();
	}
      }
      
      int xw_side=0;
      for (int xw_wall=0; xw_wall<2; ++xw_wall) {
	for (int xw_column=0; xw_column<2; ++xw_column) {
	  for (int xw_row=0; xw_row<16; ++xw_row) {
	    int id = 520 + xw_side*2*2*16 + xw_wall*2*16 + xw_column*16 + xw_row;
	    ombox[id].Draw("l");
	    if (draw_omid)
	      omid_text_v[id].Draw();
	    else if (draw_omnum)
	      omnum_text_v[id].Draw();
	    if ((draw_content && content[id]!=0) || text_was_set)
	      content_text_v[id].Draw();
	  }
	}
      }
      
      int gv_side=0;
      for (int gv_wall=0; gv_wall<2; ++gv_wall) {
	for (int gv_column=0; gv_column<16; ++gv_column) {
	  int id = 520 + 128 + gv_side*2*16 + gv_wall*16 + gv_column;
	  ombox[id].Draw("l");
	  if (draw_omid) // if ((gv_column % 5) == 0)
	    omid_text_v[id].Draw();
	  else if (draw_omnum)
	    omnum_text_v[id].Draw();
	  if ((draw_content && content[id]!=0) || text_was_set)
	    content_text_v[id].Draw();
	}
      }

      label_it->Draw();

      if (palette_axis) palette_axis->Draw();

      canvas_it->SetEditable(false);

      /////////////
      // Draw FR //
      /////////////

      canvas_fr->cd();
      canvas_fr->SetEditable(true);
      
      mw_side=1;
      for (int mw_column=0; mw_column<20; ++mw_column) {
	for (int mw_row=0; mw_row<13; ++mw_row) {
	  int id = mw_side*20*13 + mw_column*13 + mw_row;
	  ombox[id].Draw("l");
	  if (draw_omid)
	      omid_text_v[id].Draw();
	  else if (draw_omnum)
	    omnum_text_v[id].Draw();
	  if ((draw_content && content[id]!=0) || text_was_set)
	    content_text_v[id].Draw();
	}
      }
      
      xw_side=1;
      for (int xw_wall=0; xw_wall<2; ++xw_wall) {
	for (int xw_column=0; xw_column<2; ++xw_column) {
	  for (int xw_row=0; xw_row<16; ++xw_row) {
	    int id = 520 + xw_side*2*2*16 + xw_wall*2*16 + xw_column*16 + xw_row;
	    ombox[id].Draw("l");
	    if (draw_omid)
	      omid_text_v[id].Draw();
	    else if (draw_omnum)
	      omnum_text_v[id].Draw();
	    if ((draw_content && content[id]!=0) || text_was_set)
	      content_text_v[id].Draw();
	  }
	}
      }
      
      gv_side=1;
      for (int gv_wall=0; gv_wall<2; ++gv_wall) {
	for (int gv_column=0; gv_column<16; ++gv_column) {
	  int id = 520 + 128 + gv_side*2*16 + gv_wall*16 + gv_column;
	  ombox[id].Draw("l");
	  if (draw_omid)
	    omid_text_v[id].Draw();
	  else if (draw_omnum)
	    omnum_text_v[id].Draw();
	  if ((draw_content && content[id]!=0) || text_was_set)
	    content_text_v[id].Draw();
	}
      }

      label_fr->Draw();

      if (palette_axis) palette_axis->Draw();

      canvas_fr->SetEditable(false);

      text_was_set = false;

      //

      update();
    }

    
    void reset() {
      for (int omnum=0; omnum<nb_om; ++omnum)
	content[omnum] = 0;

      for (int omnum=0; omnum<nb_om; ++omnum)
	ombox[omnum].SetFillColor(0);
      
      canvas_it->Modified();
      canvas_it->Update();

      canvas_fr->Modified();
      canvas_fr->Update();

      gSystem->ProcessEvents();
    }

    float getcontent (int omnum)
    {
      return content[omnum];
    }

    void setcontent (int omnum, float value)
    {
      if ((omnum >= 0) && (omnum < nb_om))
	content[omnum] = value;
      else printf("*** wrong OM NUM\n");
    }
    
    void setcontent (int om_side, int om_wall, int om_column, int om_row, float value)
    {
      int omnum = -1;

      // auto detect MW
      if ((om_side!=-1) && (om_wall==-1) && (om_column!=-1) && (om_row!=-1))
	omnum = 260*om_side + 13*om_column + om_row;

      // auto detect XW
      else if ((om_side!=-1) && (om_wall!=-1) && (om_column!=-1) && (om_row!=-1))
	omnum = 520 + 64*om_side + 32*om_wall + 16*om_column + om_row;
       
      // auto detect GV
     else if ((om_side!=-1) && (om_wall!=-1) && (om_column!=-1) && (om_row==-1))
	omnum = 520 + 128 + 32*om_side + 16*om_wall + om_column;

      else {
	printf("+++ sndisplay: skipping OM (%d.%d.%d.%d)\n", om_side, om_wall, om_column, om_row);
	return;}

      content[omnum] = value;
    }

    void setcolor (int omnum, Color_t color)
    {
      if ((omnum >= 0) && (omnum < nb_om))
	{
	  ombox[omnum].SetFillColor(color);
	  color_was_set = true;
	}
      else printf("*** wrong OM NUM\n");
    }

    void settext (int omnum, const char *text)
    {
      if ((omnum >= 0) && (omnum < nb_om))
	{
	  content_text_v[omnum].SetText(content_text_v[omnum].GetX(), content_text_v[omnum].GetY(), text);
	  text_was_set = true;
	}
      else printf("*** wrong OM NUM\n");
    }

    void setmwcolor (int om_side, int om_column, int om_row, Color_t color)
    {
      setcolor(260*om_side + 13*om_column + om_row, color);
    }

    void setmwtext (int om_side, int om_column, int om_row, const char *text)
    {
      settext(260*om_side + 13*om_column + om_row, text);
    }
    
    void fill (int omnum, float value=1)
    {
      setcontent(omnum, content[omnum]+value);
    }

    void update ()
    {
      if (color_was_set)
	{
	  color_was_set = false;
	  return;
	}

      // autoset Z range [0, content_max]
      // unless setrange() has been called

      float content_min = content[0];
      float content_max = content[0];

      for (int omnum=1; omnum<nb_om; ++omnum)
	{
	  if (content[omnum] < content_min) content_min = content[omnum];
	  if (content[omnum] > content_max) content_max = content[omnum];
	}

      content_min = 0;
      if (range_min != -1) content_min = range_min;
      if (range_max != -1) content_max = range_max;

      for (int omnum=0; omnum<nb_om; ++omnum)
	{
	  if (content[omnum] != 0)
	    {
	      int color_index = floor (99*(content[omnum]-content_min)/(content_max-content_min));
	      if (color_index < 0) color_index = 0;
	      else if (color_index >= 100) color_index = 99;
	      ombox[omnum].SetFillColor(palette_index + color_index);
	    }
	  else
	    ombox[omnum].SetFillColor(0);
	}

      if (draw_content)
	{
	  for (int omnum=0; omnum<nb_om; ++omnum)
	    settext(omnum, Form(draw_content_format.Data(), content[omnum]));
	}

      if (palette_axis)
	{
	  palette_histo->SetMinimum(content_min);
	  palette_histo->SetMaximum(content_max);
	  palette_histo->SetContour(100);
	}

      canvas_it->Modified();
      canvas_it->Update();

      canvas_fr->Modified();
      canvas_fr->Update();

      gSystem->ProcessEvents();
    }
    
    TString calorimeter_name;

    // draw options
    bool draw_omid;
    bool draw_omnum;
    bool draw_content;
    TString draw_content_format;
    float range_min, range_max;

    bool color_was_set;
    bool text_was_set;
    
    TCanvas *canvas_it;
    TCanvas *canvas_fr;

    TText *label_it;
    TText *label_fr;

    int palette_index;
    TH2D *palette_histo;
    TPaletteAxis *palette_axis;

    std::vector<float> content;

    std::vector<TBox>  ombox;
    std::vector<TText> omid_text_v;
    std::vector<TText> omnum_text_v;
    std::vector<TText> content_text_v;

  }; // sndisplay::calorimeter class

  ////////////////////////
  // sndisplay::tracker //
  ////////////////////////

  class tracker
  {
  public:
    tracker (const char *n = "") : tracker_name (n)
    {
      canvas = nullptr;

      draw_cellid = false;
      draw_cellnum = false;
      draw_content = false;
      draw_content_format = "%.0f";

      range_min = range_max = -1;
      
      for (int cellnum=0; cellnum<nb_cell; ++cellnum)
	content.push_back(0);

      range_min = range_max = -1;

      const double spacerx = 0.0125;
      const double spacery = 0.0500;

      const double cell_sizex = (1-2*spacerx)/113.0;
      const double cell_sizey = (1-3*spacery)/(9*2);

      //////////////////////////
      // CELLS initialisation //
      //////////////////////////

      for (int cell_side=0; cell_side<2; ++cell_side) {

	  for (int cell_row=0; cell_row<113; ++cell_row) {

	    double x1 = spacerx + cell_row*cell_sizex;

	    for (int cell_layer=0; cell_layer<9; ++cell_layer) {

	    int cellnum = cell_side*113*9 + cell_row*9 + cell_layer;

	    double y1 = spacery;

	    if (cell_side == 0)
	      y1 += 9*cell_sizey + spacery + cell_layer*cell_sizey;
	    else y1 += (8-cell_layer)*cell_sizey;

	    double x2 = x1 + cell_sizex;
	    double y2 = y1 + cell_sizey;

	    TBox *box = new TBox(x1, y1, x2, y2);
	    box->SetFillColor(0);
	    box->SetLineWidth(1);
	    cellbox.push_back(box);

	    TString cellid_string = Form("M:%1d.%d.%d", cell_side, cell_layer, cell_row);
	    TText *cellid_text = new TText (x1+0.5*cell_sizex, y1+0.667*cell_sizey, cellid_string);
	    cellid_text->SetTextSize(0.01);
	    cellid_text->SetTextAlign(22);
	    cellid_text_v.push_back(cellid_text);

	    TString cellnum_string = Form("%03d", cellnum);
	    TText *cellnum_text = new TText (x1+0.5*cell_sizex, y1+0.333*cell_sizey, cellnum_string);
	    cellnum_text->SetTextSize(0.01);
	    cellnum_text->SetTextAlign(22);
	    cellnum_text_v.push_back(cellnum_text);

	    TText *content_text = new TText (x1+0.5*cell_sizex, y1+0.333*cell_sizey, "");
	    content_text->SetTextSize(0.01);
	    content_text->SetTextAlign(22);
	    content_text_v.push_back(content_text);

	    } // for cell_layer

	    if ((cell_row %5) == 0)
	      {
		TText *row_text = new TText (x1+0.5*cell_sizex, 0.5, Form("%d",cell_row));
		row_text->SetTextSize(0.03);
		row_text->SetTextAngle(90);
		row_text->SetTextAlign(22);
		row_text_v.push_back(row_text);
	      }
	  } // for cell_row

      } // for cell_side

      label_it = new TText (spacerx, 2.25*spacery+2*9*cell_sizey, "ITALY");
      label_it->SetTextSize(0.036);

      // label_fr = new TText (0.5 + spacerx, spacery+gv_sizey+spacery+13*mw_sizey+spacery+0.25*gv_sizey, "FRANCE");
      label_fr = new TText (spacerx, 0.25*spacery, "FRANCE");
      label_fr->SetTextSize(0.036);

      const Int_t nRGBs = 6;
      Double_t stops[nRGBs] = { 0.00, 0.20, 0.40, 0.60, 0.80, 1.00 };
      Double_t red[nRGBs]   = { 0.25, 0.00, 0.20, 1.00, 1.00, 0.90 };
      Double_t green[nRGBs] = { 0.25, 0.80, 1.00, 1.00, 0.80, 0.00 };
      Double_t blue[nRGBs]  = { 1.00, 1.00, 0.20, 0.00, 0.00, 0.00 };

      palette_index = TColor::CreateGradientColorTable(nRGBs, stops, red, green, blue, 100);
    };

    ~tracker() {};

    static const int nb_cell  = 2034;

    void setrange(float zmin, float zmax) 
    {
      range_min = zmin; range_max = zmax;
    }

    void draw_cellid_label() {
      draw_cellid = true;}

    void draw_cellnum_label() {
      draw_cellnum = true;}

    void draw_content_label(const char *format="%.0f") {
      draw_content_format = TString(format);
      draw_content = true;}

    void draw()
    {
      if (canvas == nullptr)
	canvas = new TCanvas (Form("C_%s",tracker_name.Data()), tracker_name, 1800, 360);

      if (draw_content)
	{
	  for (int cellnum=0; cellnum<nb_cell; ++cellnum)
	    {
	      TText *ttext = content_text_v[cellnum];
	      ttext->SetText(ttext->GetX(), ttext->GetY(), Form(draw_content_format.Data(), content[cellnum]));
	    }
	}

      canvas->cd();
      canvas->SetEditable(true);

      for (int cell_side=0; cell_side<2; ++cell_side) {

	for (int cell_row=0; cell_row<113; ++cell_row) {

	  for (int cell_layer=0; cell_layer<9; ++cell_layer) {

	    int cellnum = cell_side*113*9 + cell_row*9 + cell_layer;

	    cellbox[cellnum]->Draw("l");

	    if (draw_cellid)
	      cellid_text_v[cellnum]->Draw();

	    if (draw_cellnum)
	      cellnum_text_v[cellnum]->Draw();

	    if (draw_content && content[cellnum]!=0)
	      content_text_v[cellnum]->Draw();
	  }

	}

      }

      for (TText *row_text : row_text_v)
	row_text->Draw();

      label_it->Draw();
      label_fr->Draw();

      canvas->SetEditable(false);

      update();
    }

    void reset() {
      for (int cellnum=0; cellnum<nb_cell; ++cellnum)
	content[cellnum] = 0;

      for (int cellnum=0; cellnum<nb_cell; ++cellnum)
	cellbox[cellnum]->SetFillColor(0);

      canvas->Modified();
      canvas->Update();

      gSystem->ProcessEvents();
    }

    float getcontent (int cellnum)
    {
      return content[cellnum];
    }

    void setcontent (int cellnum, float value)
    {
      if (cellnum < nb_cell) content[cellnum] = value;
      else printf("*** wrong cell ID\n");
    }

    void setcontent (int cell_side, int cell_row, int cell_layer, float value)
    {
      int cellnum = cell_side*9*113 + cell_row*9 + cell_layer;
      setcontent(cellnum, value);
    }

    void setcolor (int cell_num, Color_t color)
    {
      if (cell_num < 2034) cellbox[cell_num]->SetFillColor(color);
      else printf("*** wrong cell ID\n");
    }

    void setcolor (int cell_side, int cell_row, int cell_layer, Color_t color)
    {
      int cell_num = cell_side*9*113 + cell_row*9 + cell_layer;
      setcolor(cell_num, color);
    }

    void fill (int cellnum, float value=1)
    {
      setcontent(cellnum, content[cellnum]+value);
    }

    void update()
    {
      // autoset Z range [0, content_max]
      // unless setrange() has been called

      float content_min = content[0];
      float content_max = content[0];

      for (int cellnum=1; cellnum<nb_cell; ++cellnum)
	{
	  if (content[cellnum] < content_min) content_min = content[cellnum];
	  if (content[cellnum] > content_max) content_max = content[cellnum];
	}

      content_min = 0;
      if (range_min != -1) content_min = range_min;
      if (range_max != -1) content_max = range_max;
      printf("Z range = [%f, %f] for '%s'\n", content_min, content_max, tracker_name.Data());

      for (int cellnum=0; cellnum<nb_cell; ++cellnum)
	{
	  if (content[cellnum] != 0)
	    {
	      int color_index = floor (99*(content[cellnum]-content_min)/(content_max-content_min));
	      if (color_index < 0) color_index = 0;
	      else if (color_index >= 100) color_index = 99;
	      cellbox[cellnum]->SetFillColor(palette_index + color_index);
	    }
	  else
	    cellbox[cellnum]->SetFillColor(0);
	}

      canvas->Modified();
      canvas->Update();

      gSystem->ProcessEvents();
    }

    TString tracker_name;

    float range_min, range_max;

    std::vector<float> content;
    std::vector<TBox*>   cellbox;

    bool draw_cellid;
    bool draw_cellnum;
    bool draw_content;
    TString draw_content_format;

    std::vector<TText*> cellid_text_v;
    std::vector<TText*> cellnum_text_v;
    std::vector<TText*> content_text_v;
    std::vector<TText*> row_text_v;

    TText *label_it;
    TText *label_fr;

    TCanvas *canvas;

    int palette_index;

  }; // sndisplay::tracker class

  /////////////////////////////
  // sndisplay::demonstrator //
  /////////////////////////////

  class demonstrator
  {
  public:
    demonstrator (const char *n = "") : demonstrator_name (n)
    {
      range_min = range_max = -1;

      // TOP_VIEW //

      const double spacerx = 0.0125;
      const double spacery = 0.0250;

      const double mw_sizey = (1-2*spacery)/(2.0 + 4*1.035 + 0.313);
      const double xw_sizey = 1.035*mw_sizey;
      const double se_sizey = 0.313*mw_sizey;
      const double gg_sizey = (1-2*spacery-2*mw_sizey-se_sizey)/18.0;

      const double mw_sizex = (1-2*spacerx)/(20 + 2*0.5*0.720);
      const double xw_sizex = (1-2*spacerx-20*mw_sizex);
      const double se_sizex = (1-2*spacerx-2*xw_sizex);
      const double gg_sizex = se_sizex/113.0;

      // printf("gg_sizex = %f\n", gg_sizex);
      // printf("gg_sizey = %f\n", gg_sizey);

      // MW (column only)

      for (int mw_side=0; mw_side<2; ++mw_side) {

	for (int mw_column=0; mw_column<20; ++mw_column) {

	  double x1 = spacerx + 0.5*xw_sizex + mw_column*mw_sizex;
	  double y1 = spacery + (1-mw_side)*(mw_sizey+4*xw_sizey+se_sizey);

	  double x2 = x1 + mw_sizex;

	  double y2 = y1 + mw_sizey;

	  top_om_content.push_back(0);

	  TBox *box = new TBox(x1, y1, x2, y2);
	  box->SetFillColor(0);
	  box->SetLineWidth(1);
	  top_om_box.push_back(box);
	  
	  TString omid_string = Form("M:%1d.%d.*", mw_side, mw_column);
	  TText *omid_text = new TText (x1+0.5*mw_sizex, y1+0.667*mw_sizey, omid_string);
	  omid_text->SetTextSize(0.032);
	  omid_text->SetTextAlign(22);
	  top_om_text.push_back(omid_text);

	  // TText *content_text = new TText (x1+0.5*mw_sizex, y1+0.333*mw_sizey, "");
	  // content_text->SetTextSize(0.02);
	  // content_text->SetTextAlign(22);
	  // content_text_v.push_back(content_text);

	} // for mw_column
	
      } // for mw_side

      // XW (column only)

      for (int xw_side=0; xw_side<2; ++xw_side) {

	for (int xw_wall=0; xw_wall<2; ++xw_wall) {

	  for (int xw_column=0; xw_column<2; ++xw_column) {

	    double x1 = spacerx + xw_wall*(xw_sizex+113*gg_sizex);
	    double x2 = x1 + xw_sizex;

	    double y1 = spacery + mw_sizey;

	    if (xw_side == 0)
	      y1 += 2*xw_sizey + se_sizey + xw_column*xw_sizey;
	    else y1 += (1-xw_column)*xw_sizey;

	    double y2 = y1 + xw_sizey;

	    top_om_content.push_back(0);

	    TBox *box = new TBox(x1, y1, x2, y2);
	    box->SetFillColor(0);
	    box->SetLineWidth(1);
	    top_om_box.push_back(box);
	  
	    TString omid_string = Form("X:%1d.%1d.%1d.*", xw_side, xw_wall, xw_column);
	    TText *omid_text = new TText (x1+0.5*xw_sizex, y1+0.6*xw_sizey, omid_string);
	    omid_text->SetTextSize(0.032);
	    omid_text->SetTextAlign(22);
	    top_om_text.push_back(omid_text);

	  }
	}
      }

      for (int gg_side=0; gg_side<2; ++gg_side) {

	  for (int gg_row=0; gg_row<113; ++gg_row) {

	    for (int gg_layer=0; gg_layer<9; ++gg_layer) {

	      double x1 = spacerx + xw_sizex + gg_row*gg_sizex;
	    
	      double y1 = spacery + mw_sizey;
	      if (gg_side == 0)
		y1 += 9*gg_sizey + se_sizey + gg_layer*gg_sizey;
	      else 
		y1 += (8-gg_layer)*gg_sizey;

	      double x2 = x1 + gg_sizex;
	      double y2 = y1 + gg_sizey;

	      top_gg_content.push_back(0);

	      TBox *box = new TBox(x1, y1, x2, y2);
	      box->SetFillColor(0);
	      box->SetLineWidth(1);
	      top_gg_box.push_back(box);

	      TEllipse *ellipse = new TEllipse((x1+x2)/2, (y1+y2)/2, gg_sizex/2, gg_sizey/2);
	      ellipse->SetFillColor(0);
	      ellipse->SetLineWidth(1);
	      top_gg_ellipse.push_back(ellipse);
	    }
	  }
      }

      const Int_t nRGBs = 6;
      Double_t stops[nRGBs] = { 0.00, 0.20, 0.40, 0.60, 0.80, 1.00 };
      Double_t red[nRGBs]   = { 0.25, 0.00, 0.20, 1.00, 1.00, 0.90 };
      Double_t green[nRGBs] = { 0.25, 0.80, 1.00, 1.00, 0.80, 0.00 };
      Double_t blue[nRGBs]  = { 1.00, 1.00, 0.20, 0.00, 0.00, 0.00 };
      
      palette_index = TColor::CreateGradientColorTable(nRGBs, stops, red, green, blue, 100);

    } // demonstrator ()


    void setrange(float zmin, float zmax) 
    {
      range_min = zmin; range_max = zmax;
    }


    void draw_top()
    {
      if (demonstrator_canvas == nullptr)
	demonstrator_canvas = new TCanvas (Form("C_demonstrator_%s",demonstrator_name.Data()), Form("%s",demonstrator_name.Data()), 1800, 450);
      else demonstrator_canvas->cd();

      for (int mw_side=0; mw_side<2; ++mw_side)
	{
	  for (int mw_column=0; mw_column<20; ++mw_column)
	    {
	      int top_om_num = mw_side*20 + mw_column;
 	      top_om_box[top_om_num]->Draw("l");
	      top_om_text[top_om_num]->Draw();
	    }
	}

      for (int xw_side=0; xw_side<2; ++xw_side)
	{
	  for (int xw_wall=0; xw_wall<2; ++xw_wall)
	    {
	      for (int xw_column=0; xw_column<2; ++xw_column)
		{
		  int top_om_num = 40 + xw_side*2*2 + xw_wall*2 + xw_column;
		  top_om_box[top_om_num]->Draw("l");
		  top_om_text[top_om_num]->Draw();
		}
	    }
	}
      
      for (int gg_side=0; gg_side<2; ++gg_side)
	{
	  for (int gg_row=0; gg_row<113; ++gg_row)
	    {
	      for (int gg_layer=0; gg_layer<9; ++gg_layer)
		{
		  int top_gg_num = gg_side*113*9 + gg_row*9 + gg_layer;
		  top_gg_box[top_gg_num]->Draw("l");
		  top_gg_ellipse[top_gg_num]->Draw("l");
		  // top_g_text[top_gg_num]->Draw();
		}
	    }
	}

      update();

    } // draw_top

    void setomcontent (int om_num, float value)
    {
      int top_om_num = -1;
      
      if (om_num < 260) // MW IT
	{
	  int om_side = 0;
	  int om_column = (om_num/13);
	  top_om_num = om_side*20 + om_column;
	}
      else if (om_num < 520) // MW IT
	{
	  int om_side = 1;
	  int om_column = (om_num-260)/13;
	  top_om_num = om_side*20 + om_column;
	}
      else if (om_num < 648) // XW
	{
	  int om_side = (om_num < 584) ? 0 : 1;
	  int om_wall = (om_num-520-om_side*64)/32;
	  int om_column = (om_num-520-om_side*64-om_wall*32)/16;
	  top_om_num = 40 + om_side*2*2 + om_wall*2 + om_column;
	}

      top_om_content[top_om_num] = value;
    }


    void setggcontent (int cell_num, float value)
    {
      if (cell_num < 2034) top_gg_content[cell_num] = value;
      else printf("*** wrong cell ID\n");
    }
    
    void setggcontent (int cell_side, int cell_row, int cell_layer, float value)
    {
      int cell_num = cell_side*9*113 + cell_row*9 + cell_layer;
      setggcontent(cell_num, value);
    }
    
    void setggcolor (int cell_num, Color_t color)
    {
      if (cell_num < 2034) top_gg_ellipse[cell_num]->SetFillColor(color);
      else printf("*** wrong cell ID\n");
    }
    
    void setggcolor (int cell_side, int cell_row, int cell_layer, Color_t color)
    {
      int cell_num = cell_side*9*113 + cell_row*9 + cell_layer;
      setggcolor(cell_num, color);
    }

    


    void reset()
    {
      for (size_t om=0; om<top_om_content.size(); ++om)
	{
	  top_om_content[om] = 0;
	  top_om_box[om]->SetFillColor(0);
	}

      for (size_t gg=0; gg<top_gg_content.size(); ++gg)
	{
	  top_gg_content[gg] = 0;
	  top_gg_ellipse[gg]->SetFillColor(0);
	  // top_gg_box[gg]->SetFillColor(0);
	}

      demonstrator_canvas->Modified();
      demonstrator_canvas->Update();

      gSystem->ProcessEvents();
    }

    void update()
    {
      demonstrator_canvas->cd();

      float top_content_min = top_om_content[0];
      float top_content_max = top_om_content[0];

      for (size_t om=0; om<top_om_content.size(); ++om)
    	{
    	  if (top_om_content[om] < top_content_min) top_content_min = top_om_content[om];
    	  if (top_om_content[om] > top_content_max) top_content_max = top_om_content[om];
    	}

      for (size_t gg=0; gg<top_gg_content.size(); ++gg)
    	{
    	  if (top_gg_content[gg] < top_content_min) top_content_min = top_gg_content[gg];
    	  if (top_gg_content[gg] > top_content_max) top_content_max = top_gg_content[gg];
    	}

      top_content_min = 0;
      if (range_min != -1) top_content_min = range_min;
      if (range_max != -1) top_content_max = range_max;
      // printf("Z range = [%f, %f] for '%s'\n", top_content_min, top_content_max, demonstrator_name.Data());

      for (size_t om=0; om<top_om_content.size(); ++om)
    	{
    	  if (top_om_content[om] != 0)
    	    {
    	      int color_index = floor (99*(top_om_content[om]-top_content_min)/(top_content_max-top_content_min));
    	      if (color_index < 0) color_index = 0;
    	      else if (color_index >= 100) color_index = 99;
    	      top_om_box[om]->SetFillColor(palette_index + color_index);
    	    }
    	  else
    	    top_om_box[om]->SetFillColor(0);
    	}

      for (size_t gg=0; gg<top_gg_content.size(); ++gg)
    	{
    	  if (top_gg_content[gg] != 0)
    	    {
    	      int color_index = floor (99*(top_gg_content[gg]-top_content_min)/(top_content_max-top_content_min));
    	      if (color_index < 0) color_index = 0;
    	      else if (color_index >= 100) color_index = 99;
    	      top_gg_ellipse[gg]->SetFillColor(palette_index + color_index);
    	    }
    	  else
    	    top_gg_ellipse[gg]->SetFillColor(0);
    	}

      demonstrator_canvas->Modified();
      demonstrator_canvas->Update();

      gSystem->ProcessEvents();
    }

    //

    TString demonstrator_name;

    float range_min, range_max;
    
    std::vector<float> top_om_content;
    std::vector<TBox*> top_om_box;
    std::vector<TText*> top_om_text;

    std::vector<float> top_gg_content;
    std::vector<TBox*> top_gg_box;
    std::vector<TEllipse*> top_gg_ellipse;
    // std::vector<TText*>  top_gg_text;

    TCanvas *demonstrator_canvas;

    int palette_index;

  }; // sndisplay::demonstrator class

} // sndisplay namespace

////////////////////////////////////////////////////////////////

// sndisplay_calorimeter_test_values()
// => sndisplay::calorimeter usage by filling OMs content with random value

void sndisplay_calorimeter_test_values (bool with_palette = true)
{
  sndisplay::calorimeter *sncalo = new sndisplay::calorimeter ("sndiplay_test", with_palette);

  sncalo->draw_content_label("%.1f");

  TRandom trand;

  for (int omnum=0; omnum<520; ++omnum) // MW
    sncalo->setcontent(omnum, trand.Gaus(85, 5));

  for (int omnum=520; omnum<648; ++omnum) // XW
    sncalo->setcontent(omnum, trand.Gaus(50, 5));

  for (int omnum=648; omnum<712; ++omnum) // GV
    sncalo->setcontent(omnum, trand.Gaus(20, 5));

  sncalo->setrange(0, 100); // to force z axis range

  sncalo->draw();

  sncalo->canvas_it->SaveAs("sndisplay-calorimeter-test-it.png");
  sncalo->canvas_fr->SaveAs("sndisplay-calorimeter-test-fr.png");

  // merge IT and FR canvas side by side using image magick (if installed)
  gSystem->Exec("which convert > /dev/null && convert sndisplay-calorimeter-test-it.png sndisplay-calorimeter-test-fr.png +append sndisplay-calorimeter-test.png");
}

////////////////////////////////////////////////////////////////

// sndisplay_calorimeter_test_status()
// => sndisplay::calorimeter usage by filling color and/or text content

void sndisplay_calorimeter_test_status ()
{
  sndisplay::calorimeter *sncalo = new sndisplay::calorimeter ("dead_channel");

  // OMs with "white photocathode PMT"
  sncalo->setmwcolor(0,  5, 9, kOrange+1); sncalo->setmwtext(0,  5, 9, "WP");
  sncalo->setmwcolor(0,  6, 2, kOrange+1); sncalo->setmwtext(0,  6, 2, "WP");
  sncalo->setmwcolor(0,  7, 7, kOrange+1); sncalo->setmwtext(0,  7, 7, "WP");
  sncalo->setmwcolor(0,  9, 2, kOrange+1); sncalo->setmwtext(0,  9, 2, "WP");
  sncalo->setmwcolor(0, 10, 6, kOrange+1); sncalo->setmwtext(0, 10, 6, "WP");
  sncalo->setmwcolor(1,  9, 5, kOrange+1); sncalo->setmwtext(1,  9, 5, "WP");
  sncalo->setmwcolor(1, 15, 6, kOrange+1); sncalo->setmwtext(1, 15, 6, "WP");

  sncalo->draw();

  sncalo->canvas_it->SaveAs("sndisplay-calorimeter-status-it.png");
  sncalo->canvas_fr->SaveAs("sndisplay-calorimeter-status-fr.png");

  // merge IT and FR canvas side by side using image magick (if installed)
  gSystem->Exec("which convert > /dev/null && convert sndisplay-calorimeter-status-it.png sndisplay-calorimeter-status-fr.png +append sndisplay-calorimeter-status.png");
}

////////////////////////////////////////////////////////////////

// sndisplay_calorimeter_test_omnum()
// => sndisplay::calorimeter usage to show the conversion OM_ID <=> OM_NUM

void sndisplay_calorimeter_test_omnum ()
{
  sndisplay::calorimeter *sncalo = new sndisplay::calorimeter ("omid_vs_omnum");

  for (int omnum=0; omnum<712; ++omnum)
    sncalo->settext(omnum, Form("%d", omnum));

  sncalo->draw();

  sncalo->canvas_it->SaveAs("sndisplay-calorimeter-omnum-it.png");
  sncalo->canvas_fr->SaveAs("sndisplay-calorimeter-omnum-fr.png");

  // merge IT and FR canvas side by side using image magick (if installed)
  gSystem->Exec("which convert > /dev/null  && convert sndisplay-calorimeter-omnum-it.png sndisplay-calorimeter-omnum-fr.png +append sndisplay-calorimeter-omnum.png");
}

////////////////////////////////////////////////////////////////

void sndisplay_cellnum ()
{
  sndisplay::tracker *sntracker = new sndisplay::tracker;

  // sntracker->draw_cellid_label();
  // sntracker->draw_cellnum_label();

  TRandom trand;

  for (int cellnum=0; cellnum<2034; ++cellnum)
    sntracker->setcontent(cellnum, trand.Gaus(100, 10));

  sntracker->draw();
}

////////////////////////////////////////////////////////////////

// main() can be compiled with (ROOT's HistPainter library must be added!)
// g++ sndisplay.cc -o sndisplay `root-config --cflags --libs` -lHistPainter

int main()
{
  sndisplay_calorimeter_test_values();
  sndisplay_calorimeter_test_status();
  sndisplay_calorimeter_test_omnum();

  return 0;
}
