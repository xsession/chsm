import { Gui } from './modules/gui.mjs';
import { Model } from './modules/model.js';
import { state_machine } from './data.js';


class App {
	constructor(model)
	{
		this.model = new Model(model);
		this.gui = new Gui();
		this.drawing = null;
		this.drag_data = {};
		this.resize_data = {};
		this.tr_draw_data = {};
		this.mouse_pos = [0, 0];
		
		this.model.states().map(this.render_state, this);
		this.model.transitions().map(this.render_transiton, this);
		
		document.querySelector('body').addEventListener("keydown", event => {
			if (event.isComposing || event.keyCode === 229) {
				return;
			}
			this.dispatch('KEYDOWN', event);
		});

		this.gui.add_event_handler('mousemove', event => {
			this.mouse_pos = this.gui.get_absolute_pos(event);
			this.dispatch('MOUSEMOVE', event);
		});

		this.gui.add_event_handler('mouseup', event => {
			this.dispatch('MOUSEUP', event);
		});

		this.gui.add_event_handler('click', event => {
			this.dispatch('CLICK', event);
		});

		this.state = this.idle_state;
	}

	dispatch(event, data)
	{
		this.state(event, data);
	}

	idle_state(event, data)
	{
		switch(event)
		{
			case 'KEYDOWN':
				switch(data.code)
				{
					case 'KeyS':
						this.create_state(this.mouse_pos);
						break;

					case 'KeyT':
						this.start_transition();
						this.state = this.select_tr_start_state;
				}
				break;

			case 'STATE_DRAG':
				this.state_drag_start(data.event, data.id);
				this.state = this.state_dragging_state;
				break;

			case 'STATE_RESIZE':
				this.state_resize_start(data.event, data.id);
				this.state = this.state_resizing_state;
				break;

			case 'TR_DRAG':
				this.trans_drag_start(data.event, data.id);
				this.state = this.transition_dragging_state;
				break;
		}
	}

	start_transition()
	{
		this.gui.set_cursor('crosshair');
	}

	select_tr_start_state(event, data)
	{
		switch(event)
		{
			case 'KEYDOWN':
				switch(data.code)
				{
					case 'Escape':
						this.gui.set_cursor('auto');
						this.state = this.idle_state;
						break;
				}
				break;

			case 'STATE_BORDER_CLICK':
				data.event.stopPropagation();
				const pos = this.gui.get_state_rel_pos(data.event, data.id);
				const t = this.model.make_new_transition();
				if (this.model.set_transition_start(t, data.id, pos))
				{
					this.tr_draw_data.trans_id = t;
					this.render_transiton(t);
					this.state = this.transition_drawing_state;
				}
				else
				{
					this.model.delete_transition(t);
				}
				console.log('start_click');
				break;
		}
	}

	transition_drawing_state(event, data)
	{
		switch(event)
		{
			case 'KEYDOWN':
				switch(data.code)
				{
					case 'Escape':
						this.model.delete_transition(this.tr_draw_data.trans_id);
						this.gui.delete_transition(this.tr_draw_data.trans_id);
						this.gui.set_cursor('auto');
						this.state = this.idle_state;
						break;

					case 'Space':
						this.model.switch_transition_elbow(this.tr_draw_data.trans_id, this.mouse_pos);
						this.redraw_transition(this.tr_draw_data.trans_id);
						break;

					case 'Backspace':
						data.preventDefault();
						this.model.remove_transition_vertex(this.tr_draw_data.trans_id, this.mouse_pos);
						this.redraw_transition(this.tr_draw_data.trans_id);
						break;
				}
				break;

			case 'MOUSEMOVE':
				this.model.set_transition_endpoint(this.tr_draw_data.trans_id, this.mouse_pos);
				this.redraw_transition(this.tr_draw_data.trans_id);
				break

			case 'CLICK':
				console.log('draw_click');
				this.model.add_transition_vertex(this.tr_draw_data.trans_id);
				break;
			
			case 'STATE_BORDER_CLICK':
				const pos = this.gui.get_state_rel_pos(data.event, data.id);
				console.log(pos);
				if (this.model.set_transition_end(this.tr_draw_data.trans_id, data.id, pos))
				{
					this.gui.set_cursor('auto');
					this.state = this.idle_state;
				}
				break;
		}
	}

	transition_dragging_state(event, data)
	{
		switch(event)
		{
			case 'MOUSEMOVE':
				this.trans_drag(data);
				break

			case 'MOUSEUP':
				this.trans_drag_end(data);
				this.state = this.idle_state;
				break;
		}
	}

	state_resizing_state(event, data)
	{
		switch(event)
		{
			case 'MOUSEMOVE':
				this.state_resize(data);
				break

			case 'MOUSEUP':
				this.state_resize_end(data);
				this.state = this.idle_state;
				break;
		}
	}

	state_dragging_state(event, data)
	{
		switch(event)
		{
			case 'MOUSEMOVE':
				this.state_drag(data);
				break

			case 'MOUSEUP':
				this.state_drag_end(data);
				this.state = this.idle_state;
				break;
		}
	}

	create_state(pos)
	{
		const state_id = this.model.make_new_state(pos);
		this.render_state(state_id);
	}

	state_drag_start(evt, state_id)
	{
		evt.preventDefault();
		const [ex, ey] = this.gui.get_absolute_pos(evt, state_id);
		const [sx, sy] = this.model.get_state(state_id).pos;
		this.drag_data.offset = [ex-sx, ey-sy];
		this.drag_data.state_id = state_id;
	}
	
	state_resize_start(evt, state_id)
	{
		evt.preventDefault();

		this.resize_data.transitions = this.model.state_transitions(state_id);
		this.resize_data.state_id = state_id;
	}
	
	state_resize(evt)
	{
		evt.preventDefault();
		const state_id = this.drag_data.state_id;
		const size = this.gui.get_state_rel_pos(evt, state_id);
		this.model.resize_state(state_id, size);
		this.gui.states[state_id].resize(this.model.get_state(state_id).size);
		this.resize_data.transitions.map(
			(t) => {
				this.model.update_transition_path(t);
				this.redraw_transition(t);
			});
	}

	state_resize_end(evt)
	{
		evt.preventDefault();
	}

	state_drag(evt)
	{
		evt.preventDefault();
		const [ex, ey] = this.gui.get_absolute_pos(evt);
		const [ox, oy] = this.drag_data.offset;
		const pos = [ex-ox, ey-oy];
		const state_id = this.drag_data.state_id;
		this.model.move_state(state_id, pos);
	
		this.model.changes.states.map((id) => {
			this.gui.states[id].move(this.model.get_state(id).pos);
			}, this);


		this.model.changes.transitions.map(this.redraw_transition, this);

		this.model.ack_changes();
	}

	state_drag_end(evt)
	{
		evt.preventDefault();
		this.model.update_parent(this.drag_data.state_id);
	}

	render_state(state_id)
	{
		const state = this.model.get_state(state_id);

		this.gui.render_state(state_id,
			state.title,
			state.pos,
			state.size,
			state.text,
			this.model.options.text_height,
			(evt) => {this.dispatch('STATE_DRAG', {event: evt, id: state_id});},
			(evt) => {this.dispatch('STATE_RESIZE', {event: evt, id: state_id});},
			(evt) => {this.dispatch('STATE_BORDER_CLICK', {event: evt, id: state_id});});
	}

	trans_drag_start(evt, trans_id)
	{
		evt.preventDefault();

		const p = this.gui.get_absolute_pos(evt);
		const line = this.model.transition_get_line_index(trans_id, p);

		this.drag_data.trans_id = trans_id;
		this.drag_data.line = line;
		this.drag_data.label_width = this.gui.get_path_label_size(trans_id)[0];
	}

	trans_drag(evt)
	{
		evt.preventDefault();

		const p = this.gui.get_absolute_pos(evt);
		const trans_id = this.drag_data.trans_id;
		const line = this.drag_data.line;
		this.model.transition_drag(trans_id, line, p, this.drag_data.label_width);
		this.redraw_transition(trans_id);
	}

	trans_drag_end(evt)
	{
		evt.preventDefault();
	}

	render_transiton(trans_id)
	{
		const tr = this.model.get_transition(trans_id);
		this.gui.render_transition(
			trans_id,
			tr.vertices,
			tr.label,
			tr.label_pos,
			(evt) => {this.dispatch('TR_DRAG', {event: evt, id: trans_id});});
	}

	redraw_transition(trans_id)
	{
		const tr = this.model.get_transition(trans_id);
		this.gui.redraw_path_with_arrow(trans_id, tr.vertices, tr.label, tr.label_pos);
	}
}

window.addEventListener('DOMContentLoaded', (event) => {window.app = new App(state_machine)});
